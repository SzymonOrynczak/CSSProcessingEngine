#pragma once

int nOfCSSSections(struct ListOfElements headAndTail);
void nOfSelectorInSectionIth(struct ListOfElements headAndTail, int ith, char* command);
void nOfAttrInSectionIth(struct ListOfElements headAndTail, int ith, char* command);
void jthSelectorInIthSection(ListOfElements headAndTail, int ith, int jth);
void valOfAttrInIthSection(ListOfElements headAndTail, int ith, char* attrName);
void nOfOccurancesOfAttrNameInAllSectionCSS(ListOfElements headAndTail, char* attrName);
void nOfSelectorZ(ListOfElements headAndTail, char* selectorName);
void valOfAttrNInSelectorZ(struct ListOfElements headAndTail, char* attrN, char* selectorZ);
void removeIthSection(struct ListOfElements* headAndTail, int ith, int ifPrint);
void removeAttrNameFromIthSection(struct ListOfElements* headAndTail, int ith, char* attrName, int ifPrint);