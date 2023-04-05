#include "Structs.h"
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include "Commands.h"
#include <cstring>


#define _CRT_SECURE_NO_WARNINGS

/*Spis elementów rozpoznawanych w pliku
#define COMA 1 
#define LBRACE 2
#define RBRACE 3
#define COLON 4
#define SEMICOLON 5
#define WORDNAME 6
#define ATTRVALUE 7
*/


/*Statusy w tracie czytania pliku*/
#define READINGSELECTORS 0
#define READINGATTRNAME 1
#define READINGATTRVALUE 2
#define READINGCOMMANDS 3

#define N 256
#define RESUME "****"


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
	int questionMarkCounter;
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


void processQuestionMark(struct StatusOfReading* state)
{
	if (state->status == READINGSELECTORS)
	{
		state->questionMarkCounter++;

		if (state->questionMarkCounter == 4)
		{
			state->status = READINGCOMMANDS;
			state->questionMarkCounter = 0;
		}
	}

	else if (state->status == READINGCOMMANDS)
	{
		state->charTable[state->charLenght] = '?';
		state->charLenght++;
	}

	else
	{
		state->attrValue[state->attrLen] = '?';
		state->attrLen++;
	}
}


int ifNumber(const char* command)
{
	int commandLen = strlen(command);

	for (int i = 0; i < commandLen - 4; i++)
	{
		if (!(command[i] >= '0' && command[i] <= '9'))
		{
			return 0;
		}
	}

	return 1;
}


int endingWith(char* command, const char* ending)
{
	int endingLen = strlen(ending);
	int commandLen = strlen(command);

	for (int i = endingLen; i >= 1; i--)
	{
		if (ending[endingLen - i] != command[commandLen - i])
		{
			return 0;
		}
	}

	return 1;
}


int createNumber(const char* command)
{
	int commandLen = strlen(command);
	int newNumber = 0;

	for (int i = 0; i < commandLen - 4; i++)
	{
		newNumber = newNumber * 10 + (command[i] - '0');
	}

	return newNumber;
}


//algorytm naiwnego wyszukiwania substringa
int ifSubString(char* command, const char* substring)
{
	int commmandLen = strlen(command);
	int substringLen = strlen(substring);
	
	for (int i = 0; i <= commmandLen - substringLen; i++)
	{
		int found = 1;

		for (int j = 0; j < substringLen; j++)
		{
			if (substring[j] != command[i+j])
			{
				found = 0;
			}
		}
		if (found == 1)
		{
			return 1;
		}
	}

	return 0;
}


int createNumberToComa(char* command)
{
	int i = 0;
	int newNumber = 0;

	while (command[i] != ',')
	{
		newNumber = newNumber * 10 + (command[i] - '0');
		i++;
	}

	return newNumber;
}


void createEndingName(char* command)
{
	int comaIndex = 0;
	int i = strlen(command) - 1;

	while (command[i] != ',')
	{
		i--;
	}

	comaIndex = i;

	for (int j = comaIndex + 1; j < strlen(command); j++)
	{
		command[j - comaIndex - 1] = command[j];
	}

	command[strlen(command) - comaIndex - 1] = '\0';
}


int createNumberFromComa(char* command)
{
	int comaIndex = 0;
	int i = strlen(command) - 1;
	int newNumber = 0;

	while (command[i] != ',')
	{
		i--;
	}

	comaIndex = i;

	for (int j = comaIndex + 1; j < strlen(command); j++)
	{
		newNumber = newNumber * 10 + (command[j] - '0');
	}

	return newNumber;
}


void executeCommand(struct StatusOfReading* state, struct ListOfElements* headAndTail)
{
	char* command = state->charTable;

	if (strcmp(command, "?") == 0)
	{
		nOfCSSSections(*headAndTail);
	}

	//i,S,? - print the number of selectors for block number i (block and attribute numbers start from 1), if there is no such block, skip;
	else if (endingWith(command, ",S,?") && ifNumber(command))
	{
		int newNumber = createNumber(command);
		nOfSelectorInSectionIth(*headAndTail, newNumber);
	}

	//i,A,? - print the number of attributes for block number i, if there is no such block, skip;
	else if (endingWith(command, ",A,?") && ifNumber(command))
	{
		int newNumber = createNumber(command);
		nOfAttrInSectionIth(*headAndTail, newNumber);
	}

	//n, A, ? -print the total(for all blocks) number of occurrences of attribute named n(duplicates should be removed when reading).It can be 0;
	else if (endingWith(command, ",A,?"))
	{
		command[strlen(command) - 4] = '\0';
		nOfOccurancesOfAttrNameInAllSectionCSS(*headAndTail, command);
	}

	//z,S,? - print the total (for all blocks) number of occurrences of selector z. It can be 0;
	else if (endingWith(command, ",S,?"))
	{
		command[strlen(command) - 4] = '\0';
		nOfSelectorZ(*headAndTail, command);
	}

	//i, D, * -remove the entire block number i(i.e., separators + attributes), after successful execution, print "deleted";
	else if (endingWith(command, ",D,*"))
	{
		int newNumber = createNumber(command);
		printf("Remove section %d\n", newNumber);
		removeIthSection(headAndTail, newNumber);
		printList(headAndTail->head);
	}

	//i,A,n - print the value of the attribute with the name n for the i-th block, if there is no such attribute or block, skip;
	else if (ifSubString(command, ",A,"))
	{
		int newNumber = createNumberToComa(command);
		createEndingName(command); //szukanie ostatniego przecinka
		valOfAttrInIthSection(*headAndTail, newNumber, command);
	}

	//i, D, n - remove the attribute named n from the i - th block, if the block becomes empty
	//as a result of the operation, it should also be removed(along with any selectors), after successful
	//execution, print "deleted".
	else if (ifSubString(command, ",D,"))
	{
		int newNumber = createNumberToComa(command);
		createEndingName(command);
		removeAttrNameFromIthSection(headAndTail, newNumber, command);
		printList(headAndTail->head);
	}

	//z,E,n - print the value of the attribute named n for the selector z, in case of multiple occurrences of selector z, take the last one.
	//If there is no such attribute, skip;
	else if (ifSubString(command, ",E,"))
	{
		char z[N];
		int i = 0;

		while (command[i] != ',')
		{
			z[i] = command[i];
			i++;
		}
		z[i] = '\0';
		
		createEndingName(command);

		valOfAttrNInSelectorZ(*headAndTail, command, z);
	}


	else if (ifSubString(command, ",S,"))
	{
		int ith = createNumberToComa(command);
		int jth = createNumberFromComa(command);
		jthSelectorInIthSection(*headAndTail, ith, jth);
	}
}


void processCommand(struct StatusOfReading* state, struct ListOfElements* headAndTail)
{
	if (state->charLenght == 0)
	{
		return;
	}

	state->charTable[state->charLenght] = '\0';
	state->charLenght++;

	printf("Procesujemy komende: %s \n", state->charTable);

	if (strcmp(state->charTable, RESUME) == 0)
	{
		state->status = READINGSELECTORS;
	}

	else
	{
		printf("Wykonujemy komende: %s\n", state->charTable);
		executeCommand(state, headAndTail);
	}

	state->charLenght = 0;
}


void recognizeChar(char readChar, struct StatusOfReading* state, struct ListOfElements* headAndTail)
{
	if (state->status == READINGCOMMANDS)
	{
		if (readChar == '\n')
		{
			processCommand(state, headAndTail);
		}

		else
		{
			state->charTable[state->charLenght] = readChar;
			state->charLenght++;
		}
	}

	else if (readChar == '?')
	{
		processQuestionMark(state);
	}

	else if (readChar == ',')
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
	state.questionMarkCounter = 0;

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
	nOfSelectorZ(headAndTail, t1);
	
	valOfAttrNInSelectorZ(headAndTail, t0, t2);

	removeIthSection(&headAndTail, 1);

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