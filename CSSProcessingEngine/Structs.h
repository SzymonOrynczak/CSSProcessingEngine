﻿#pragma once
#define BLOCKSIZE 8

struct Attribute
{
	char* attributeName;
	char* attributeValue;
	struct Attribute* next;
};


struct Selector
{
	char* selectorName;
	struct Selector* next;
};


struct SectionCSS
{
	struct Selector* headSelector;
	struct Attribute* headAttribute;
};


struct Element
{
	struct SectionCSS sectionTab[BLOCKSIZE];
	int nOfOccpied;
	struct Element* next;
	struct Element* prev;
};


struct ListOfElements
{
	struct Element* head;
	struct Element* tail;
};


void addSelector(struct SectionCSS* css, char* selectorName, int selectorLen);
void addAttribute(struct SectionCSS* css, char* attr, int attrLen, char* value, int valueLen);
void addToList(struct ListOfElements* headAndTail, struct SectionCSS css);
struct Element* freeListMemory(struct Element* head);