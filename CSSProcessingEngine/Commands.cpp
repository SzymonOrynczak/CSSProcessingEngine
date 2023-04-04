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


void removeAttrFromSection(struct SectionCSS* section, char* attrName)
{
	struct Attribute* attrToRemove = section->headAttribute;
	struct Attribute* prevAttr = NULL;

	while (attrToRemove != NULL && strcmp(attrName, attrToRemove->attributeName) != 0)
	{
		prevAttr = attrToRemove;
		attrToRemove = attrToRemove->next;
	}

	if (attrToRemove == NULL)
	{
		printf("Skip - brak atrybutu o nazwie %s", attrName);
	}

	else
	{
		if (prevAttr == NULL)
		{
			//usuwamy glowe
			section->headAttribute = section->headAttribute->next;
		}

		else
		{
			prevAttr = attrToRemove->next;
		}
		free(attrToRemove);
	}
}


void removeAttrNameFromIthSection(struct ListOfElements* headAndTail, int ith, char* attrName)
{
	struct SectionCSS* section = findIthSection(*headAndTail, ith);
	removeAttrFromSection(section, attrName);

	if (section->headAttribute == NULL)
	{
		deleteIthSection(headAndTail, ith);
	}
}


void removeListsFromSection(struct SectionCSS* sectionToRemove)
{
	while (sectionToRemove->headAttribute != NULL)
	{
		struct Attribute* attrToRemove = sectionToRemove->headAttribute;

		sectionToRemove->headAttribute = sectionToRemove->headAttribute->next;

		free(attrToRemove);
	}

	while (sectionToRemove->headSelector != NULL)
	{
		struct Selector* selectorToRemove = sectionToRemove->headSelector;

		sectionToRemove->headSelector = sectionToRemove->headSelector->next;

		free(selectorToRemove);
	}
}


void deleteIthSection(struct ListOfElements* headAndTail, int ith)
{
	struct Element* current = headAndTail->head;
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
		return;
	}

	int desiredNumber = counter - ith;
	int desiredIndex = current->nOfOccpied - desiredNumber - 1;

	struct SectionCSS sectionToRemove = current->sectionTab[desiredIndex];

	removeListsFromSection(&sectionToRemove);

	//z bloku current usuwamy desiredNumber

	for (int i = desiredIndex + 1; i < current->nOfOccpied; i++)
	{
		current->sectionTab[i - 1] = current->sectionTab[i];
	}

	current->nOfOccpied--;

	if (current->nOfOccpied == 0)
	{
		//znika blok current
		struct Element* currP = current->prev;
		struct Element* currN = current->next;

		if (currP != NULL && currN != NULL)
		{
			currP->next = currN;
			currN->prev = currP;
		}

		else if (currP == NULL && currN != NULL)
		{
			//current jest glowa

			currN->prev = NULL;
			headAndTail->head = currN;
		}

		else if (currP != NULL && currN == NULL)
		{
			//current to tail

			currP->next = NULL;
			headAndTail->tail = currP;
		}

		else
		{
			//current jest jedyny na liscie
			headAndTail->head = NULL;
			headAndTail->tail = NULL;
		}

		free(current);
	}
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


int ifAttrNameInSection(struct SectionCSS section, char* attrName)
{
	struct Attribute* attr = section.headAttribute;

	while (attr != NULL)
	{
		if (strcmp(attrName, attr->attributeName) == 0)
		{
			return 1;
		}

		attr = attr->next;
	}

	return 0;
}


void nOfOccurancesOfAttrNameInAllSectionCSS(struct ListOfElements headAndTail, char* attrName)
{
	struct Element* current = headAndTail.head;
	int nameCounter = 0;
	
	while (current != NULL)
	{
		for (int i = 0; i < current->nOfOccpied; i++)
		{
			struct SectionCSS section = current->sectionTab[i];
			if (ifAttrNameInSection(section, attrName))
			{
				nameCounter++;
			}
		}

		current = current->next;
	}

	printf("Ilosc wystapien nazwy %s: %d \n", attrName, nameCounter);
}


	/*TODO: Liczy prawidłowo dla pierwszej sekcji CSS, jest problem z przejściem do nastepnej sekcji - currentAttr wykrywa NULL na
	* końcu sekcji i nie czyta atrybutów kolejnej sekcji.
	* 
	* - nOfOccurancesOfAttrNameInAllSectionCSS - naprawić
	* - zrobić obsługę delete
	* - zrobić wszystko oprócz z usuwaniem
	*/		


// prawie tak samo jak ta wyżej
void nOfSelectorZ(struct ListOfElements headAndTail, char* selectorName)
{
	struct Element* current = headAndTail.head;
	int nameCounter = 0;

	while (current != NULL)
	{
		if (strcmp(selectorName, current->sectionTab->headSelector->selectorName) == 0)
		{
			nameCounter++;
		}
		current = current->next;
	}

	printf("Selektor o nazwie %s wystapil %d razy \n", selectorName, nameCounter);
}


int ifSelectorInSection(struct SectionCSS section, char* nameOfSelector)
{
	struct Selector* currSelector = section.headSelector;

	while (currSelector != NULL)
	{
		if (strcmp(nameOfSelector, currSelector->selectorName) == 0)
		{
			return 1;
		}

		currSelector = currSelector->next;
	}

	return 0;
}


void printValOfAttrName(struct SectionCSS section, char* attrN, char* selectorZ)
{
	struct Attribute* attr = section.headAttribute;

	while (attr != NULL)
	{
		if (strcmp(attrN, attr->attributeName) == 0)
		{
			printf("Wartosc atrybutu %s z selectora %s: %s\n", attrN, selectorZ, attr->attributeValue);
			return;
		}
		attr = attr->next;
	}

	printf("Skip - brak atrybutu %s w selektorze %s\n", attrN, selectorZ);
}


void valOfAttrNInSelectorZ(struct ListOfElements headAndTail, char* attrN, char* selectorZ)
{
	struct Element* current = headAndTail.tail;

	while (current != NULL)
	{
		for (int i = current->nOfOccpied - 1; i >= 0; i--)
		{
			struct SectionCSS section = current->sectionTab[i];

			if (ifSelectorInSection(section, selectorZ))
			{
				printValOfAttrName(section, attrN, selectorZ);
				return;
			}

		}
		current = current->prev;
	}
}