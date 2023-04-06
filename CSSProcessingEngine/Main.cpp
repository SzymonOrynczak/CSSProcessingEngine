#include "Structs.h"
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include "Commands.h"
#include <cstring>

#define _CRT_SECURE_NO_WARNINGS

/*Statuses of file reading*/
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
		state->charTable[state->charLenght] = '\0';
		state->charLenght++;
		int nOfRemoved = removeEndWhiteChar(state->charTable, state->charLenght);
		state->charLenght -= nOfRemoved;
		addSelector(&(state->currentBuildCSS), state->charTable, state->charLenght);
		state->charLenght = 0;
	}

	else if (state->status == READINGATTRVALUE)
	{
		state->attrValue[state->attrLen] = ',';
		state->attrLen++;
	}
}


void processLBrace(struct StatusOfReading* state)
{
	if (state->status == READINGSELECTORS)
	{
		state->charTable[state->charLenght] = '\0';
		state->charLenght++;
		int nOfRemoved = removeEndWhiteChar(state->charTable, state->charLenght);
		state->charLenght -= nOfRemoved;
		addSelector(&(state->currentBuildCSS), state->charTable, state->charLenght);
		state->charLenght = 0;
		state->status = READINGATTRNAME;
	}
}


void processColon(struct StatusOfReading* state)
{
	if (state->status == READINGATTRNAME)
	{
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
}


void processSemiColon(struct StatusOfReading* state)
{
	if (state->status == READINGATTRVALUE)
	{
		state->attrValue[state->attrLen] = '\0';
		state->attrLen++;
		int nOfRemoved = removeEndWhiteChar(state->attrValue, state->attrLen);
		state->attrLen -= nOfRemoved;
		addAttribute(&(state->currentBuildCSS), state->charTable, state->charLenght, state->attrValue, state->attrLen);
		state->status = READINGATTRNAME;
		state->charLenght = 0;
	}
}


void processRBrace(struct StatusOfReading* state, struct ListOfElements* headAndTail)
{
	if (state->status == READINGATTRVALUE)
	{
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
		addToList(headAndTail, state->currentBuildCSS);
		state->status = READINGSELECTORS;

		state->currentBuildCSS.headSelector = NULL;
		state->currentBuildCSS.headAttribute = NULL;
	}
}


void processWhiteSpace(struct StatusOfReading* state, char letter)
{
	if (state->status == READINGATTRVALUE)
	{
		if (state->attrLen == 0)
		{
			
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
		int result = nOfCSSSections(*headAndTail);
		printf("? == %d\n", result);
	}

	//i,S,? - print the number of selectors for block number i (block and attribute numbers start from 1), if there is no such block, skip;
	else if (endingWith(command, ",S,?") && ifNumber(command))
	{
		int newNumber = createNumber(command);
		nOfSelectorInSectionIth(*headAndTail, newNumber, command);
	}

	//i,A,? - print the number of attributes for block number i, if there is no such block, skip;
	else if (endingWith(command, ",A,?") && ifNumber(command))
	{
		int newNumber = createNumber(command);
		nOfAttrInSectionIth(*headAndTail, newNumber, command);
	}

	//n, A, ? -print the total(for all blocks) number of occurrences of attribute named n(duplicates should be removed when reading).It can be 0;
	else if (endingWith(command, ",A,?"))
	{
		command[strlen(command) - 4] = '\0';
		nOfOccurancesOfAttrNameInAllSectionCSS(*headAndTail, command); //here command is attrName
	}

	else if (endingWith(command, ",S,?"))
	{
		command[strlen(command) - 4] = '\0';
		nOfSelectorZ(*headAndTail, command);
	}

	else if (endingWith(command, ",D,*"))
	{
		int newNumber = createNumber(command);
		removeIthSection(headAndTail, newNumber, 1);
	}

	else if (ifSubString(command, ",A,"))
	{
		int newNumber = createNumberToComa(command);
		createEndingName(command); //szukanie ostatniego przecinka
		valOfAttrInIthSection(*headAndTail, newNumber, command);
	}

	else if (ifSubString(command, ",D,"))
	{
		int newNumber = createNumberToComa(command);
		createEndingName(command);
		removeAttrNameFromIthSection(headAndTail, newNumber, command, 0);
	}

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

	if (strcmp(state->charTable, RESUME) == 0)
	{
		state->status = READINGSELECTORS;
	}

	else
	{
		executeCommand(state, headAndTail);
	}

	state->charLenght = 0;
}


void processEndOfFile(struct StatusOfReading* state, struct ListOfElements* headAndTail)
{
	if (state->status == READINGCOMMANDS)
	{
		processCommand(state, headAndTail);
	}
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
		processComa(state);
	}

	else if (readChar == '{')
	{
		processLBrace(state);
	}

	else if (readChar == ':')
	{
		processColon(state);
	}

	else if (readChar == ';')
	{
		processSemiColon(state);
	}

	else if (readChar == '}')
	{
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

		if (readChar == '\n')
		{
			state->lineCounter++;
			state->charCounter = 0;
		}

		recognizeChar(readChar, state, headAndTail);
		counter = scanf("%c", &readChar);
	}
	processEndOfFile(state, headAndTail);
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

	headAndTail.head = freeListMemory(headAndTail.head);
	headAndTail.tail = NULL;

	return 0;
}