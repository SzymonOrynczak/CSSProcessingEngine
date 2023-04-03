#pragma once
#include "Structs.h"
#include "Commands.h"
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>


struct SectionCSS* findIthSection(struct ListOfElements headAndTail, int ith)
{
	struct Element* current = headAndTail.head;
	int counter = current->nOfOccpied;

	while (current != NULL && counter < ith)
	{
		current = current->next;

		if (current != NULL)
		{
			counter += current->nOfOccpied;
		}
	}

	if (current == NULL)
	{
		printf("Skip - brak sekcji css o numerze %d \n", ith);
		return NULL;
	}

	int desiredNumber = counter - ith;
	int desiredIndex = current->nOfOccpied - desiredNumber - 1;

	return &(current->sectionTab[desiredIndex]);
}


int nOfCSSSections(struct ListOfElements headAndTail)
{
	struct Element* current = headAndTail.head;
	int counter = 0;

	while (current != NULL)
	{
		counter += current->nOfOccpied;
		current = current->next; 
	}
	
	printf("Ilosc css wynosi: %d \n", counter);
	return counter;
}


int nOfSelectorList(struct Selector* head)
{
	struct Selector* current = head;
	int counter = 0;

	while (current != NULL)
	{
		counter++;
		current = current->next;
	}

	return counter;
}


void nOfSelectorInSectionIth(struct ListOfElements headAndTail, int ith)
{
	struct Element* current = headAndTail.head;
	int counter = current->nOfOccpied;

	while (current != NULL && counter < ith)
	{
		current = current->next;

		if(current != NULL)
		{
			counter += current->nOfOccpied;
		}
	}

	if (current == NULL)
	{
		printf("Skip - brak sekcji css o numerze %d \n", ith);
	}

	else
	{
		int desiredNumber = counter - ith;
		int desiredIndex = current->nOfOccpied - desiredNumber - 1;
		int nOfSelectors = nOfSelectorList(current->sectionTab[desiredIndex].headSelector);
		printf("Liczba selectorow w %d sekcji css: %d \n", ith, nOfSelectors);
	}
}


int nOfAttrList(struct Attribute* head)
{
	struct Attribute* current = head;
	int counter = 0;

	while (current != NULL)
	{
		counter++;
		current = current->next;
	}

	return counter;
}


void nOfAttrInSectionIth(struct ListOfElements headAndTail, int ith)
{
	struct Element* current = headAndTail.head;
	int counter = current->nOfOccpied;

	while (current != NULL && counter < ith)
	{
		current = current->next;

		if (current != NULL)
		{
			counter += current->nOfOccpied;
		}
	}

	if (current == NULL)
	{
		printf("Skip - brak sekcji css o numerze %d \n", ith);
	}

	else
	{
		int desiredNumber = counter - ith;
		int desiredIndex = current->nOfOccpied - desiredNumber - 1;
		int nOfAttr = nOfAttrList(current->sectionTab[desiredIndex].headAttribute);
		printf("Liczba atrybutow w %d sekcji css: %d \n", ith, nOfAttr);
	}
}


void jthSelectorInIthSection(ListOfElements headAndTail, int ith, int jth)
{
	struct SectionCSS* sectionCss = findIthSection(headAndTail, ith);
	if (sectionCss == NULL)
	{
		return;
	}

	struct Selector* current = sectionCss->headSelector;
	int counter = 1;

	while (current != NULL && counter < jth)
	{
		counter++;
		current = current->next;
	}

	if (current == NULL)
	{
		printf("Skip - brak selektora %d \n", jth);
	}

	else
	{
		printf("Selektor: %s \n", current->selectorName);
	}
}


void valOfAttrInIthSection(ListOfElements headAndTail, int ith, char* attrName)
{
	struct SectionCSS* sectionCss = findIthSection(headAndTail, ith);

	if (sectionCss == NULL)
	{
		return;
	}

	struct Attribute* current = sectionCss->headAttribute;
	
	while (current != NULL && strcmp(attrName, current->attributeName) != 0)
	{
		current = current->next;
	}

	if (current == NULL)
	{
		printf("Skip - brak nazwy atrybutu %s \n", attrName);
	}

	else
	{
		printf("Atrybut o nazwie %s ma wartosc: %s \n", current->attributeName, current->attributeValue);
	}
}


void nOfOccurancesOfAttrNameInAllSectionCSS(ListOfElements headAndTail, char* attrName)
{
	struct Element* currentSection = headAndTail.head;
	struct Attribute* currentAttr = headAndTail.head->sectionTab->headAttribute;

	int attrNameCounter = 0;
	

	while (currentAttr != NULL)
	{
		printf("Jestem w whilu \n");

		if (strcmp(attrName, currentAttr->attributeName) == 0)
		{
			printf("Jestem w ifie \n");
			attrNameCounter++;
		}

		currentAttr = currentAttr->next;
	}

	//TODO: Liczy prawidłowo dla pierwszej sekcji CSS, jest problem z przejściem do nastepnej sekcji - currentAttr wykrywa NULL na 
	//		końcu sekcji i nie czyta atrybutów kolejnej sekcji.

	printf("Napotkano NULL \n");
	printf("Liczba wystapien nazwy %s: %d \n", attrName, attrNameCounter);
}
