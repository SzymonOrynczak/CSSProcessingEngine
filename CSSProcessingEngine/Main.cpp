#include "Structs.h"
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include "Commands.h"


#define _CRT_SECURE_NO_WARNINGS

/*Spis elementów rozpoznawanych w pliku*/
#define COMA 1 
#define LBRACE 2
#define RBRACE 3
#define COLON 4
#define SEMICOLON 5
#define WORDNAME 6
#define ATTRVALUE 7

/*Statusy w tracie czytania pliku*/
#define READINGSELECTORS 0
#define READINGATTRNAME 1
#define READINGATTRVALUE 2

#define N 256


struct StatusOfReading
{
	char charTable[N];
	int charLenght;
	int status;
	int charCounter;
	int lineCounter;
	struct SectionCSS currentBuildCSS;
	char attrValue[N];
	int attrLen;
};

/*
int specialCharacter(char symbol)
{
	if (symbol == '{')
	{
		return 1;
	}

	if (symbol == ',')
	{
		return 1;
	}

	if (symbol == ':')
	{
		return 1;
	}

	if (symbol == ';')
	{
		return 1;
	}

	if (symbol == '}')
	{
		return 1;
	}

	return 0;
}
*/

int ifWhiteChar(char letter)
{
	if (letter == ' ')
	{
		return 1;
	}

	if (letter == '\t')
	{
		return 1;
	}

	if (letter == '\n')
	{
		return 1;
	}

	return 0;
}


int removeEndWhiteChar(char* tab, int len)
{
	int removedChars = 0;
	int i = 1;

	while (ifWhiteChar(tab[len - 1 - i]))
	{
		removedChars++;
		i++;
	}

	tab[len - 1 - removedChars] = '\0';
	return removedChars;
}


void processComa(struct StatusOfReading* state)
{
	if (state->status == READINGSELECTORS)
	{
		printf("Koniec nazwy poprzedniego selektora i rozpoczynamy drugi selektor ");
		state->charTable[state->charLenght] = '\0';
		state->charLenght++;
		int nOfRemoved = removeEndWhiteChar(state->charTable, state->charLenght);
		state->charLenght -= nOfRemoved;
		addSelector(&(state->currentBuildCSS), state->charTable, state->charLenght);
		state->charLenght = 0;
	}

	else if (state->status == READINGATTRVALUE)
	{
		state->charTable[state->charLenght] = ',';
		state->charLenght++;
	}

	else
	{
		printf("Linia %d znak %d - Błąd: w nazwie atrybutu nie wolno przecinka \n", state->lineCounter, state->charCounter);
	}
}


void processLBrace(struct StatusOfReading* state)
{
	if (state->status == READINGSELECTORS)
	{
		printf("Koniec czytania selektora i rozpoczynamy nazwe atrybutu ");
		state->charTable[state->charLenght] = '\0';
		state->charLenght++;
		int nOfRemoved = removeEndWhiteChar(state->charTable, state->charLenght);
		state->charLenght -= nOfRemoved;
		addSelector(&(state->currentBuildCSS), state->charTable, state->charLenght);
		state->charLenght = 0;
		state->status = READINGATTRNAME;
	}

	else
	{
		printf("Linia %d znak %d - Błąd: w bloku atrybutów nie wolno { \n", state->lineCounter, state->charCounter);
	}
}


void processColon(struct StatusOfReading* state)
{
	if (state->status == READINGATTRNAME)
	{
		printf("Koniec nazwy atrybutu i zaczynamy wartosc atrybutu ");
		state->charTable[state->charLenght] = '\0';
		state->charLenght++;
		int nOfRemoved = removeEndWhiteChar(state->charTable, state->charLenght);
		state->charLenght -= nOfRemoved;
		state->status = READINGATTRVALUE;
		state->attrLen = 0;
	}

	else if (state->status == READINGSELECTORS)
	{
		state->charTable[state->charLenght] = ':';
		state->charLenght++;
	}

	else
	{
		printf("Linia %d znak %d - Błąd: w bloku wartosci atrybutu nie wolno : \n", state->lineCounter, state->charCounter);
	}
}


void processSemiColon(struct StatusOfReading* state)
{
	if (state->status == READINGATTRVALUE)
	{
		printf("Koniec wartosci atrybutu i zaczynamy nazwe kolejnego atrybutu ");
		state->attrValue[state->attrLen] = '\0';
		state->attrLen++;
		int nOfRemoved = removeEndWhiteChar(state->attrValue, state->attrLen);
		state->attrLen -= nOfRemoved;
		addAttribute(&(state->currentBuildCSS), state->charTable, state->charLenght, state->attrValue, state->attrLen);
		state->status = READINGATTRNAME;
		state->charLenght = 0;
	}

	else
	{
		printf("Linia %d znak %d - Błąd: w bloku selektora lub nazwy atrybutu nie wolno ; \n", state->lineCounter, state->charCounter);
	}
}


void processRBrace(struct StatusOfReading* state, struct ListOfElements* headAndTail)
{
	if (state->status == READINGATTRVALUE)
	{
		printf("Koniec wartosci atrybutu i zaczynamy nazwe selektora ");

		state->attrValue[state->attrLen] = '\0';
		state->attrLen++;

		int nOfRemoved = removeEndWhiteChar(state->attrValue, state->attrLen);
		state->attrLen -= nOfRemoved;

		addAttribute(&(state->currentBuildCSS), state->charTable, state->charLenght, state->attrValue, state->attrLen);

		state->status = READINGATTRNAME;

		state->charLenght = 0;
	}

	if (state->status == READINGATTRNAME && state->charLenght == 0)
	{
		printf("Current CSS juz sie zbudowal i prawa klamra go zakonczyla \n ");
		addToList(headAndTail, state->currentBuildCSS);
		printOneSection(state->currentBuildCSS);

		state->status = READINGSELECTORS;

		state->currentBuildCSS.headSelector = NULL;
		state->currentBuildCSS.headAttribute = NULL;
	}

	else
	{
		printf("Linia %d znak %d - Błąd: w bloku selektora, nazwy atrybutu lub w wartości nie wolno } \n", state->lineCounter, state->charCounter);
	}
}

/*
void endWord(struct StatusOfReading* state)
{
	if (state->status == READINGWORDNAME)
	{
		state->charTable[state->charLenght] = '\0'; //koniec napisu
		printf("Koniec elementu WORDNAME %s \n", state->charTable);

		state->status = READINGDEFAULT;
		state->charLenght = 0;
	}
}
*/

void processWhiteSpace(struct StatusOfReading* state, char letter)
{
	if (state->status == READINGATTRVALUE)
	{
		if (state->attrLen == 0)
		{
			printf("Bialy znak zostal wyciety ");
		}

		else
		{
			state->attrValue[state->attrLen] = letter;
			state->attrLen++;
		}
	}

	else
	{
		if (state->charLenght == 0)
		{
			printf("Bialy znak zostal wyciety ");
		}

		else
		{
			state->charTable[state->charLenght] = letter;
			state->charLenght++;
		}
	}
}


void processLetters(struct StatusOfReading* state, char letter)
{
	if (state->status == READINGATTRVALUE)
	{
		state->attrValue[state->attrLen] = letter;
		state->attrLen++;
	}
	
	else
	{
		state->charTable[state->charLenght] = letter;
		state->charLenght++;
	}
}


void recognizeChar(char readChar, struct StatusOfReading* state, struct ListOfElements* headAndTail)
{
	if (readChar == ',')
	{
		printf("Rozpoznany COMA ,");
		processComa(state);
	}

	else if (readChar == '{')
	{
		printf("Rozpoznany LBRACE {");
		processLBrace(state);
	}

	else if (readChar == ':')
	{
		printf("Rozpoznany COLON :");
		processColon(state);
	}

	else if (readChar == ';')
	{
		printf("Rozpoznany SEMICOLON ;");
		processSemiColon(state);
	}

	else if (readChar == '}')
	{
		printf("Rozpoznany RBRACE }");
		processRBrace(state, headAndTail);
	}
	

	else if (readChar == ' ' || readChar == '\n' || readChar == '\t')
	{
		processWhiteSpace(state, readChar);
	}

	else
	{
		processLetters(state, readChar);
	}
}


void readCSS(struct StatusOfReading* state, struct ListOfElements* headAndTail)
{
	char readChar;
	int counter;

	counter = scanf("%c", &readChar);

	while (counter > 0)
	{
		state->charCounter++;

		printf("Przeczytany znak w pozycji (linia = %d, charCounter = %d): %c \n", state->lineCounter, state->charCounter, readChar);

		if (readChar == '\n')
		{
			state->lineCounter++;
			state->charCounter = 0;
		}


		recognizeChar(readChar, state, headAndTail);
		counter = scanf("%c", &readChar);
	}

	printf("Przeczytany koniec pliku.\n");
}



int main()
{
	struct StatusOfReading state;
	state.status = READINGSELECTORS;
	state.charLenght = 0;
	state.charCounter = 0;
	state.lineCounter = 1;
	state.currentBuildCSS.headAttribute = NULL;
	state.currentBuildCSS.headSelector = NULL;

	ListOfElements headAndTail;
	headAndTail.head = NULL;
	headAndTail.tail = NULL;

	readCSS(&state, &headAndTail);

	nOfCSSSections(headAndTail);
	nOfSelectorInSectionIth(headAndTail, 3);
	nOfAttrInSectionIth(headAndTail, 2);
	jthSelectorInIthSection(headAndTail, 2, 20);

	char t0[6] = { 'c', 'o', 'l', 'o', 'r', '\0'};
	valOfAttrInIthSection(headAndTail, 2, t0);

	char t1[6] = { 'w', 'i', 'd', 't', 'h', '\0'};
	nOfOccurancesOfAttrNameInAllSectionCSS(headAndTail, t1);

	char t2[3] = { 'h', '1', '\0' };
	//nOfSelectorZ(headAndTail, t2);
	
	valOfAttrNInSelectorZ(headAndTail, t0, t2);

	printList(headAndTail.head);

	deleteIthSection(&headAndTail, 1);

	removeAttrNameFromIthSection(&headAndTail, 2, t0);

	printList(headAndTail.head);


	/*
	char t0[4] = { 'a', 'b', 'c', '\0' };
	char v0[3] = { 'x', 'y', '\0' };

	char s0[3] = { 'u', 'v', '\0' };


	struct SectionCSS css0;
	css0.headSelector = NULL;
	css0.headAttribute = NULL;
	addSelector(&css0, s0, 3);
	addAttribute(&css0, t0, 4, v0, 3);


	struct SectionCSS css1;
	css1.headSelector = NULL;
	css1.headAttribute = NULL;

	struct SectionCSS css2;
	css2.headSelector = NULL;
	css2.headAttribute = NULL;

	printList(headAndTail.head);
	addToList(&headAndTail, css0);
	printList(headAndTail.head);
	addToList(&headAndTail, css1);
	printList(headAndTail.head);
	addToList(&headAndTail, css2);
	printList(headAndTail.head);
	*/


	headAndTail.head = freeListMemory(headAndTail.head);
	headAndTail.tail = NULL;

	return 0;
}