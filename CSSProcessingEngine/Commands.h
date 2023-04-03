#pragma once

int nOfCSSSections(struct ListOfElements headAndTail);
void nOfSelectorInSectionIth(struct ListOfElements headAndTail, int ith);
void nOfAttrInSectionIth(struct ListOfElements headAndTail, int ith);
void jthSelectorInIthSection(ListOfElements headAndTail, int ith, int jth);
void valOfAttrInIthSection(ListOfElements headAndTail, int ith, char* attrName);
void nOfOccurancesOfAttrNameInAllSectionCSS(ListOfElements headAndTail, char* attrName);