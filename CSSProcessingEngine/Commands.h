#pragma once

int nOfCSSSections(struct ListOfElements headAndTail);
void nOfSelectorInSectionIth(struct ListOfElements headAndTail, int ith);
void nOfAttrInSectionIth(struct ListOfElements headAndTail, int ith);
void jthSelectorInIthSection(ListOfElements headAndTail, int ith, int jth);
void valOfAttrInIthSection(ListOfElements headAndTail, int ith, char* attrName);
void nOfOccurancesOfAttrNameInAllSectionCSS(ListOfElements headAndTail, char* attrName);
void nOfSelectorZ(ListOfElements headAndTail, char* selectorName);
void valOfAttrNInSelectorZ(struct ListOfElements headAndTail, char* attrN, char* selectorZ);
void deleteIthSection(struct ListOfElements* headAndTail, int ith);
void removeAttrNameFromIthSection(struct ListOfElements* headAndTail, int ith, char* attrName);