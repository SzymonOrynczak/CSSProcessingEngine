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
		return NULL;
	}

	int desiredNumber = counter - ith;
	int desiredIndex = current->nOfOccpied - desiredNumber - 1;

	return &(current->sectionTab[desiredIndex]);
}


void removeAttrFromSection(struct SectionCSS* section, char* attrName, int ith)
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
		return;
	}

	else
	{
		if (prevAttr == NULL)
		{
			section->headAttribute = section->headAttribute->next;
		}

		else
		{
			prevAttr->next = attrToRemove->next;
		}
		free(attrToRemove);

		printf("%d,D,%s == deleted\n", ith, attrName);
	}
}


void removeAttrNameFromIthSection(struct ListOfElements* headAndTail, int ith, char* attrName, int ifPrint)
{
	struct SectionCSS* section = findIthSection(*headAndTail, ith);

	if (section == NULL) { return; }

	removeAttrFromSection(section, attrName, ith);
	

	if (section->headAttribute == NULL)
	{
		removeIthSection(headAndTail, ith, ifPrint);
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


void removeIthSection(struct ListOfElements* headAndTail, int ith, int ifPrint)
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
		return;
	}

	int desiredNumber = counter - ith;
	int desiredIndex = current->nOfOccpied - desiredNumber - 1;

	struct SectionCSS sectionToRemove = current->sectionTab[desiredIndex];

	removeListsFromSection(&sectionToRemove, ith);

	//removing desireNumber from block

	for (int i = desiredIndex + 1; i < current->nOfOccpied; i++)
	{
		current->sectionTab[i - 1] = current->sectionTab[i];
	}

	current->nOfOccpied--;

	if (current->nOfOccpied == 0)
	{
		//current disapears
		struct Element* currP = current->prev;
		struct Element* currN = current->next;

		if (currP != NULL && currN != NULL)
		{
			currP->next = currN;
			currN->prev = currP;
		}

		else if (currP == NULL && currN != NULL)
		{
			//current is head

			currN->prev = NULL;
			headAndTail->head = currN;
		}

		else if (currP != NULL && currN == NULL)
		{
			//current is tail

			currP->next = NULL;
			headAndTail->tail = currP;
		}

		else
		{
			//current is the only one on list
			headAndTail->head = NULL;
			headAndTail->tail = NULL;
		}
		free(current);		
	}

	if (ifPrint)
	{
		printf("%d,D,* == deleted\n", ith);
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


void nOfSelectorInSectionIth(struct ListOfElements headAndTail, int ith, char* command)
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
		return;
	}

	else
	{
		int desiredNumber = counter - ith;
		int desiredIndex = current->nOfOccpied - desiredNumber - 1;
		int nOfSelectors = nOfSelectorList(current->sectionTab[desiredIndex].headSelector);
		printf("%s == %d\n", command, nOfSelectors);
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


void nOfAttrInSectionIth(struct ListOfElements headAndTail, int ith, char* command)
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
		return;
	}

	else
	{
		int desiredNumber = counter - ith;
		int desiredIndex = current->nOfOccpied - desiredNumber - 1;
		int nOfAttr = nOfAttrList(current->sectionTab[desiredIndex].headAttribute);
		printf("%s == %d\n", command, nOfAttr);
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

	}

	else
	{
		printf("%d,S,%d == %s\n",ith ,jth ,current->selectorName);
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
		return;
	}

	else
	{
		printf("%d,A,%s == %s\n", ith, current->attributeName, current->attributeValue);
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
	printf("%s,A,? == %d\n", attrName, nameCounter);
}
		

int ifSelectorNameInBlock(struct SectionCSS section, char* selectorName)
{
	struct Selector* selector = section.headSelector;

	while (selector != NULL)
	{
		if (strcmp(selectorName, selector->selectorName) == 0)
		{
			return 1;
		}

		selector = selector->next;
	}

	return 0;
}


void nOfSelectorZ(struct ListOfElements headAndTail, char* selectorName)
{
	struct Element* current = headAndTail.head;
	int selectorCounter = 0;
	int i = 0;

	while (selectorName[i] != '\0')
	{
		if (selectorName[i] == ',')
		{
			return;
		}
		i++;
	}

	while (current != NULL)
	{
		for (int j = 0; j < current->nOfOccpied; j++)
		{
			struct SectionCSS section = current->sectionTab[j];
			if (ifSelectorNameInBlock(section, selectorName))
			{
				selectorCounter++;
			}
		}
		current = current->next;
	}
	printf("%s,S,? == %d\n", selectorName, selectorCounter);
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
			printf("%s,E,%s == %s\n", selectorZ, attrN ,attr->attributeValue);
			return;
		}
		attr = attr->next;
	}

	return;
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



