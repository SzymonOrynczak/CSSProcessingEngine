#include "Structs.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


void addSelector(struct SectionCSS* css, char* selectorName, int selectorLen)
{
	if (selectorLen <= 1)
	{
		return;
	}

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


void removeAttrDuplicate(struct SectionCSS* css, char* attr)
{
	struct Attribute* current = css->headAttribute;
	struct Attribute* prev = NULL;

	while (current != NULL && strcmp(current->attributeName, attr) != 0)
	{
		prev = current;
		current = current->next;
	}

	if (current != NULL)
	{
		//remove current
		if (prev != NULL)
		{
			prev->next = current->next;
		}

		else
		{
			css->headAttribute = css->headAttribute->next;
		}
		free(current);
	}

}


void addAttribute(struct SectionCSS* css, char* attr, int attrLen, char* value, int valueLen)
{
	removeAttrDuplicate(css, attr);

	struct Attribute* newAttr = (struct Attribute*)malloc(sizeof(struct Attribute));
	newAttr->attributeName = (char*)malloc(attrLen * sizeof(char));
	newAttr->attributeValue = (char*)malloc(valueLen * sizeof(char));

	newAttr->next = NULL;

	for (int i = 0; i < attrLen; i++)
	{
		newAttr->attributeName[i] = attr[i];
	}

	for (int i = 0; i < valueLen; i++)
	{
		newAttr->attributeValue[i] = value[i];
	}

	if (css->headAttribute == NULL)
	{
		css->headAttribute = newAttr;
		return;
	}

	struct Attribute* oldLast = css->headAttribute;

	while (oldLast->next != NULL)
	{
		oldLast = oldLast->next;
	}

	oldLast->next = newAttr;
}


void addToList(struct ListOfElements* headAndTail, struct SectionCSS css)
{
	if (headAndTail->tail == NULL || headAndTail->tail->nOfOccpied == BLOCKSIZE)
	{
		struct Element* newElem = (struct Element*)malloc(sizeof(struct Element));

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




struct Element* freeListMemory(struct Element* head)
{
	while (head != NULL)
	{
		struct Element* elemToRemove = head;
		head = head->next;
		free(elemToRemove);
	}

	return head;
}