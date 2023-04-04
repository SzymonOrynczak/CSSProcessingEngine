#include "Structs.h"
#include <cstdio>
#include <cstdlib>


void addSelector(struct SectionCSS* css, char* selectorName, int selectorLen)
{
	struct Selector* newSelector = (struct Selector*)malloc(sizeof(struct Selector));
	newSelector->selectorName = (char*)malloc(selectorLen * sizeof(char));

	newSelector->next = NULL;

	for (int i = 0; i < selectorLen; i++)
	{
		newSelector->selectorName[i] = selectorName[i];
	}

	if (css->headSelector == NULL)
	{
		css->headSelector = newSelector;
		return;
	}

	struct Selector* oldLast = css->headSelector;

	while (oldLast->next != NULL)
	{
		oldLast = oldLast->next;
	}

	oldLast->next = newSelector;
}

//TODO: zrobic dodawanie na koniec
void addAttribute(struct SectionCSS* css, char* attr, int attrLen, char* value, int valueLen)
{
	struct Attribute* newAttr = (struct Attribute*)malloc(sizeof(struct Attribute));
	newAttr->attributeName = (char*)malloc(attrLen * sizeof(char));
	newAttr->attributeValue = (char*)malloc(valueLen * sizeof(char));

	for (int i = 0; i < attrLen; i++)
	{
		newAttr->attributeName[i] = attr[i];
	}

	for (int i = 0; i < valueLen; i++)
	{
		newAttr->attributeValue[i] = value[i];
	}

	newAttr->next = css->headAttribute;
	css->headAttribute = newAttr;
}


void addToList(struct ListOfElements* headAndTail, struct SectionCSS css)
{
	if (headAndTail->tail == NULL || headAndTail->tail->nOfOccpied == BLOCKSIZE)
	{
		struct Element* newElem = (struct Element*)malloc(sizeof(struct Element));
		printf("Zaalokowana pamiec elementu pod adresem %d \n", newElem);

		newElem->nOfOccpied = 1;
		newElem->sectionTab[0] = css;
		newElem->next = NULL;
		newElem->prev = headAndTail->tail;


		if (headAndTail->head != NULL)
		{
			headAndTail->tail->next = newElem;
		}

		else
		{
			headAndTail->head = newElem;
		}

		headAndTail->tail = newElem;
	}

	else
	{
		headAndTail->tail->sectionTab[headAndTail->tail->nOfOccpied] = css;
		headAndTail->tail->nOfOccpied++;
	}
}


void printAttributes(struct Attribute* head)
{
	struct Attribute* current = head;

	while (current != NULL)
	{
		printf("<%s:%s> -> ", current->attributeName, current->attributeValue);
		current = current->next;
	}

	printf("NULL");
}


void printSelectors(struct Selector* head)
{
	struct Selector* current = head;

	while (current != NULL)
	{
		printf("<%s> ->", current->selectorName);
		current = current->next;
	}

	printf("NULL");
}


void printOneSection(struct SectionCSS css)
{
	printSelectors(css.headSelector);
	printf(" Attrybuty: ");
	printAttributes(css.headAttribute);
}


void printTabCSS(struct Element* current)
{
	printf(" %d: ", current->nOfOccpied);
	for (int i = 0; i < current->nOfOccpied; i++)
	{
		printf("[%d]", i);
		printOneSection(current->sectionTab[i]);
		printf(", ");
	}
}


void printList(struct Element* head)
{
	if (head == NULL)
	{
		printf("Lista css pusta \n	");
	}

	else
	{
		printf("Lista css:");
		struct Element* current = head;

		while (current != NULL)
		{
			printf("\nBlok: ");
			printf("{%d %d}", current, current->next);
			printTabCSS(current);
			printf("<->");
			current = current->next;
		}
		printf("NULL \n");

	}
}


struct Element* freeListMemory(struct Element* head)
{
	while (head != NULL)
	{
		struct Element* elemToRemove = head;
		head = head->next;

		printf("Dealokacja elementu: [%d %d] \n", elemToRemove, elemToRemove->next);
		free(elemToRemove);
	}

	return head;
}