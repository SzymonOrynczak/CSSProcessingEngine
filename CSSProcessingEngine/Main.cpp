#include "Structs.h"
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>

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
#define READINGDEFAULT 0
#define READINGWORDNAME 1
#define READINGATTRVALUE 2

#define N 256


struct StatusOfReading
{
	char charTable[N];
	int charLenght;
	int status;
	int charCounter;
	int lineCounter;
};


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


void processWhiteSpace(struct StatusOfReading* state)
{
	endWord(state);
}

void processOtherChar(struct StatusOfReading* state, char letter)
{
	if (state->status == READINGDEFAULT)
	{
		state->status = READINGWORDNAME;
	}

	state->charTable[state->charLenght] = letter;
	state->charLenght++;
}


void recognizeChar(char readChar, struct StatusOfReading* state)
{
	if (specialCharacter(readChar))
	{
		endWord(state);
	}

	if (readChar == ',')
	{
		printf("Rozpoznany COMA ,");
	}

	else if (readChar == '{')
	{
		printf("Rozpoznany LBRACE {");
	}

	else if (readChar == ':')
	{
		printf("Rozpoznany COLON :");
	}

	else if (readChar == '}')
	{
		printf("Rozpoznany RBRACE }");
	}

	else if (readChar == ';')
	{
		printf("Rozpoznany SEMICOLON ;");
	}

	else if (readChar == ' ' || readChar == '\n' || readChar == '\t')
	{
		processWhiteSpace(state);
	}

	else
	{
		processOtherChar(state, readChar);
	}
}


void readCSS(struct StatusOfReading* state)
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



		recognizeChar(readChar, state);
		counter = scanf("%c", &readChar);
	}

	printf("Przeczytany koniec pliku.\n");
}



int main()
{
	struct StatusOfReading state;
	state.status = READINGDEFAULT;
	state.charLenght = 0;
	state.charCounter = 0;
	state.lineCounter = 1;

	readCSS(&state);

	ListOfElements headAndTail;
	headAndTail.head = NULL;
	headAndTail.tail = NULL;

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

	headAndTail.head = freeListMemory(headAndTail.head);
	headAndTail.tail = NULL;

	return 0;
}