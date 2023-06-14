#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include "String.h"
using namespace std;

#define T 8
#define ENTER 10
#define BUFF 1000
#define BUFF_TMP 100
#define ENTER 10

#define CHAR_TO_INT_DIFFERENCE 48

struct selectorsList {
    char* str;
    selectorsList* next = NULL;
    selectorsList* previous = NULL;
};

struct attributeList {
    char* name;
    char* value;
    attributeList* next = NULL;
    attributeList* previous = NULL;
};

struct sectionList {
    int sectionNumber;
    selectorsList* selectors = NULL;
    attributeList* attributes = NULL;
};

struct cssList {
    sectionList section[T];
    cssList* next = NULL;
    cssList* previous = NULL;
};

int getSelectorNum(char sec[]) {
    int num = 1;
    int i = 0;
    while (sec[i] && sec[i] != '{') {
        if (sec[i] == ',' && sec[0] != '{')num++;
        i++;

    }
    return num;
}

bool checkIfLetter(char str) {
    if (str > 32 && str < 126) return true;
    return false;
}


bool checkIfWordsEqual(char* word1, char* word2) {
    int i = 0;
    int word1Len = 0;
    int word2Len = 0;
    while (word1[i++])word1Len++;
    i = 0;
    while (word2[i++])word2Len++;
    i = 0;
    if (word1Len != word2Len)return false;
    while (word1[i] && word2[i]) {
        if (word1[i] != word2[i])return false;
        i++;

    }
    return true;
}

char* getAttribute(int whitch, char attributes[], bool getName) {
    int i = 0;
    int attributeNum = 0;
    char* attributeToReturn = new char[strlen(attributes) + 1];

    //przesun sie w napisie do odpowiedniego selektora i zwroc go
    while (attributes[i] != '{') i++;
    // i++;
    while (attributes[i]) {
        if (attributes[i] == ';')attributeNum++;
        if (whitch != 0)i++;
        if (attributeNum == whitch)break;

    }
    int j = 0;
    if (getName == true) {
        i++;
        while (attributes[i] != ':' && attributes[i]) {
            if (checkIfLetter(attributes[i])) attributeToReturn[j++] = attributes[i];
            i++;

        }

    }
    else {
        while (attributes[i] != ':' && attributes[i]) {
            i++;

        }
        i++;
        ///i++;
        while (attributes[i] != ';' && attributes[i]) {
            if (j == 0) {
                if (checkIfLetter(attributes[i]))
                    attributeToReturn[j++] = attributes[i];

            }
            else attributeToReturn[j++] = attributes[i];
            i++;

        }


    }
    if (checkIfLetter(attributeToReturn[j - 1]) == false)attributeToReturn[j - 1] = '\0';
    else attributeToReturn[j] = '\0';
    return attributeToReturn;
}

char* getSelector(int which, char selectors[]) {
    if (selectors[0] == '{')return NULL;
    int i = 0;
    int selectorNum = 0;
    char* selectorToReturn = new char[strlen(selectors) + 1];

    //przesun sie w napisie do odpowiedniego selektora i zwroc go
    while (selectors[i]) {
        if (selectors[i] == ',')selectorNum++;
        if (which != 0)i++;
        if (selectorNum == which)break;


    }
    int j = 0;

    while (selectors[i] != '{' && selectors[i] && selectors[i] != ',' && selectors[i] != '\0') {
        if (j == 0) {
            if (checkIfLetter(selectors[i]))
                selectorToReturn[j++] = selectors[i];

        }
        else selectorToReturn[j++] = selectors[i];
        i++;

    }
    if (checkIfLetter(selectorToReturn[j - 1]) == false)selectorToReturn[j - 1] = '\0';
    else selectorToReturn[j] = '\0';
    return selectorToReturn;
}

int getAttributesNum(char sec[]) {
    int num = 0;
    int i = 0;
    while (sec[i]) {
        if (sec[i] == ';')num++;
        i++;

    }
    return num;
}

void createNewSelector(char sec[], selectorsList*& l, selectorsList* previousL) {
    if (sec != NULL) {
        if (l == NULL) {
            selectorsList* newSelector = new selectorsList;
            newSelector->next = NULL;
            newSelector->previous = NULL;

            newSelector->str = new char[strlen(sec) + 1];
            newSelector->str = sec;


            if (previousL != NULL)newSelector->previous = previousL;
            l = newSelector;
            if (previousL != NULL)previousL->next = l;

        }

        else {
            previousL = l;
            if (l != NULL)createNewSelector(sec, l->next, previousL);

        }

    }
}

void createNewAttribute(char sec[], attributeList*& l, attributeList* previousL, int i) {

    if (l == NULL) {
        attributeList* newAttribute = new attributeList;
        newAttribute->next = NULL;
        newAttribute->previous = NULL;

        newAttribute->name = new char[strlen(sec) + 1];
        newAttribute->value = new char[strlen(sec) + 1];

        newAttribute->name = getAttribute(i, sec, true);
        newAttribute->value = getAttribute(i, sec, false);

        if (previousL != NULL)newAttribute->previous = previousL;
        l = newAttribute;
        if (previousL != NULL)previousL->next = l;

    }
    else {
        previousL = l;
        if (l != NULL)createNewAttribute(sec, l->next, previousL, i);

    }
}

int findEmptySection(cssList* list) {
    int indexOfEmptySection = -1;
    for (int i = 0; i < T; i++) {
        if (list->section[i].selectors == NULL && list->section[i].attributes == NULL) {
            indexOfEmptySection = i;
            break;

        }

    }
    return indexOfEmptySection;
}

void createNewList(cssList*& list, cssList* previousList) {
    cssList* newList = new cssList;
    newList->next = NULL;
    for (int i = 0; i < T; i++) {
        newList->section[i].attributes = NULL;
        newList->section[i].attributes = NULL;
        newList->section[i].selectors = NULL;

    }
    if (previousList != NULL) {
        newList->previous = previousList;

    }
    list = newList;
    if (previousList != NULL) {
        previousList->next = list;

    }
}

void attributeRepetition(char* attributeName, attributeList* list, int i, char str[], bool& repetition) {
    if (list != NULL && checkIfWordsEqual(list->name, attributeName) == true) {
        list->value = getAttribute(i, str, false);
        repetition = true;

    }
    else if (list != NULL && list->next != NULL) {
        attributeRepetition(attributeName, list->next, i, str, repetition);

    }
    else
        repetition = false;
}
void selectorRepetition(char* selectorString, selectorsList* list, bool& repetition) {
    if (list != NULL && checkIfWordsEqual(list->str, selectorString) == true) {
        repetition = true;

    }
    else if (list != NULL && list->next != NULL) {
        selectorRepetition(selectorString, list->next, repetition);

    }
    else
        repetition = false;
}

void createSectionAtTheEnd(char sec[], cssList*& list, cssList* previousList) {

    if (list == NULL) {
        createNewList(list, previousList);

    }
    //find first empty section
    int indexOfEmptySection = findEmptySection(list);
    if (indexOfEmptySection == -1)createSectionAtTheEnd(sec, list->next, list);
    else {
        bool attributeRep = false;
        bool selectorRep = false;
        for (int i = 0; i < getSelectorNum(sec); i++) {
            selectorRepetition(getSelector(i, sec), list->section[indexOfEmptySection].selectors, selectorRep);
            if (selectorRep == false)
                createNewSelector(getSelector(i, sec), list->section[indexOfEmptySection].selectors, list->section[indexOfEmptySection].selectors);
            selectorRep = false;

        }


        for (int i = 0; i < getAttributesNum(sec); i++) {
            attributeRepetition(getAttribute(i, sec, true), list->section[indexOfEmptySection].attributes, i, sec, attributeRep);
            if (attributeRep == false) {
                createNewAttribute(sec, list->section[indexOfEmptySection].attributes, list->section[indexOfEmptySection].attributes, i);

            }
            attributeRep = false;


        }

    }

}
void freeSelectors(selectorsList* list) {
    if (list != NULL && list->next != NULL)freeSelectors(list->next);
    else {
        if (list->previous != NULL)list->previous->next = NULL;
        if (list->previous != NULL)freeSelectors(list->previous);
        list = NULL;
        delete list;


    }
}
void freeAttributes(attributeList* list) {
    if (list != NULL && list->next != NULL)freeAttributes(list->next);
    else {
        if (list->previous != NULL)list->previous->next = NULL;
        if (list->previous != NULL)freeAttributes(list->previous);
        list = NULL;
        delete list;


    }
}
void freeMemory(cssList* list) {
    if (list != NULL) {
        for (int i = 0; i < T; i++) {
            if (list->section[i].selectors != NULL) freeSelectors(list->section[i].selectors);
            if (list->section[i].attributes != NULL) freeAttributes(list->section[i].attributes);

        }
        if (list->next != NULL) freeMemory(list->next);

    }


}

void createSectionString(char* str, char* tmp) {
    int i = 0;
    while (str[i] != '\0') {
        i++;

    }
    int j = 0;
    if (i != 0)str[i++] = ' ';
    while (tmp[j]) {
        str[i] = tmp[j];
        i++;
        j++;

    }
}

void createCommandString(char* str, char* tmp) {
    int i = 0;
    while (str[i] != '\0') {
        i++;

    }
    int j = 0;
    if (i != 0)str[i++] = ' ';
    while (tmp[j]) {
        str[i] = tmp[j];
        i++;
        j++;

    }
}

void printSelectors(selectorsList* l) {

    if (l != NULL) {
        cout << l->str << endl;
        if (l->next != NULL)printSelectors(l->next);

    }
}

void printAttributes(attributeList* l) {

    if (l != NULL && l->name != NULL && l->value != NULL) {
        cout << l->name << l->value << endl;
        if (l->next != NULL)printAttributes(l->next);

    }
}

void clearStr(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = {
        };
        i++;

    }
}

void checkIfCommandsStart(char* str, bool& commands) {
    int i = 0;
    while (str[i] == '?') i++;
    if (i == 4) {
        clearStr(str);
        commands = true;

    }

    i = 0;
    while (str[i] == '*') i++;
    if (i == 4) {
        clearStr(str);
        commands = false;

    }
}

void countSectionAmount(cssList* list, int& sectionAmount) {
    for (int i = 0; i < T; i++) {
        if (list->section[i].attributes != NULL || list->section[i].selectors != NULL) sectionAmount++;

    }
    if (list->next != NULL)countSectionAmount(list->next, sectionAmount);
}

void printSectionAmount(cssList* list, int sectionAmount) {
    countSectionAmount(list, sectionAmount);
    cout << "? == " << sectionAmount << endl;
}

bool checkIfDigit(char c) {
    if (c > 47 && c < 58) return true;
    else return false;
}


int getIndexOfFirstNum(char* command) {
    int i = 0;
    while (command[i] != ',' && command[i])
    {
        i++;

    }
    return i;
}

int getIndexOfSecondNum(char* command) {
    int i = 0;
    while (command[i])
    {
        i++;

    }
    return i;
}

int makeNumber(char* command, int i) {
    //make number
    int number = 0;
    int j = i - 1;
    int tenNum = 1;
    while (command[j] != ',' && j >= 0 && command[j]) {
        number += (command[j] - CHAR_TO_INT_DIFFERENCE) * tenNum;
        j--;
        tenNum *= 10;

    }
    return number;
}

void selectorsNumber(selectorsList* selector, int occurrences, int num) {
    if (selector != NULL) {
        occurrences++;
        selectorsNumber(selector->next, occurrences, num);

    }
    else
        cout << num << ",S,? == " << occurrences << endl;
}

void attributesNumber(attributeList* attribute, int& occurrences, int num, bool print) {

    if (attribute != NULL && attribute->name != NULL && attribute->value != NULL) {
        occurrences++;
        attributesNumber(attribute->next, occurrences, num, print);

    }
    else if (attribute != NULL && attribute->next != NULL)attributesNumber(attribute->next, occurrences, num, print);
    else if (print == true)
        cout << num << ",A,? == " << occurrences << endl;
}

cssList* goToblock(cssList*& list, int block) {
    for (int i = 0; i < block; i++) {
        if (list->next != NULL)list = list->next;

    }
    return list;
}

bool isSectionEmpty(sectionList l) {
    if (l.attributes != NULL || l.selectors != NULL)return true;
    return false;
}

void findNSection(sectionList& l, cssList* list, int n, int sectionNumber) {
    for (int i = 0; i < T; i++) {
        if (isSectionEmpty(list->section[i]))sectionNumber++;
        if (sectionNumber == n) {
            l = list->section[i];
            break;

        }

    }
    if (sectionNumber != n && list->next != NULL)findNSection(l, list->next, n, sectionNumber);

}

void printNumberOfOccurrences(cssList* list, int num, char element) {
    sectionList l;
    findNSection(l, list, num, 0);

    //find nth section
   /*
   int block = (num - 1) / T;
    list = goToblock(list, block);
    int section = (num - 1) % T;
    */
    int numberOfOccurrences = 0;
    if (element == 'S') {
        selectorsNumber(l.selectors, numberOfOccurrences, num);

    }
    else if (element == 'A')
        attributesNumber(l.attributes, numberOfOccurrences, num, true);
}

void getSpecificSelector(int selectorNumber, int i, int sectionNumber, selectorsList* selector) {

    if (i != selectorNumber && selector != NULL) {
        i++;
        getSpecificSelector(selectorNumber, i, sectionNumber, selector->next);

    }
    else if (i == selectorNumber && selector != NULL)cout << sectionNumber << ",S," << selectorNumber + 1 << " == " << selector->str << endl;

}

void printSelector(cssList* list, int sectionNumber, int selectorNumber) {
    /*int block = (sectionNumber - 1) / T;
    list = goToblock(list, block);
    int sectionIndex = (sectionNumber - 1) % T;*/
    int i = 0;
    sectionList l;
    findNSection(l, list, sectionNumber, 0);

    getSpecificSelector(selectorNumber - 1, i, sectionNumber, l.selectors);

}

void getSpecificAttributeValue(attributeList* list, int sectionNumber, char* attributeName) {

    if (list != NULL) {
        if (list->name != NULL && list->value != NULL && checkIfWordsEqual(attributeName, list->name))cout << sectionNumber << ",A," << attributeName << " == " << list->value << endl;
        if (list->next != NULL) {
            getSpecificAttributeValue(list->next, sectionNumber, attributeName);

        }

    }

}

void printAttributeValue(cssList* list, int sectionNumber, char* attributeName) {
    /*int block = (sectionNumber - 1) / T;
    list = goToblock(list, block);*/

    //int sectionIndex = (sectionNumber - 1) % T;
    sectionList l;
    findNSection(l, list, sectionNumber, 0);
    getSpecificAttributeValue(l.attributes, sectionNumber, attributeName);

}

void getAttributeName(char* command, char*& attributeName, int index) {
    int i = index;
    int j = 0;
    while (command[i] && command[i] != ',') {
        attributeName[j++] = command[i++];

    }
    attributeName[j] = '\0';
}

bool commandPrintOccurrenccesByName(char* command, int index) {
    if ((command[index + 1] == 'A' || command[index + 1] == 'S') && command[index + 3] == '?')return true;
    return false;
}

bool commandPrintValueOfSpecAttrInSpecSel(char* command, int index) {
    if (command[index + 1] == 'E')return true;
    return false;
}

void findSelectorInSection(selectorsList* selectors, attributeList* attributes, char* selector, char* attributeName, bool& stop) {

    if (selectors != NULL && checkIfWordsEqual(selectors->str, selector)) {
        if (checkIfWordsEqual(attributes->name, attributeName)) {

            cout << selector << ",E," << attributeName << " == " << attributes->value << endl;
            stop = true;

        }
        else if (attributes->next != NULL)findSelectorInSection(selectors, attributes->next, selector, attributeName, stop);

    }//find attribute;
    else if (selectors != NULL && selectors->next != NULL)findSelectorInSection(selectors->next, attributes, selector, attributeName, stop);
}

void findSelectorInBlock(cssList* list, char* selector, char* attributeName) {
    bool stop = false;
    for (int i = T - 1; i >= 0; i--) {
        findSelectorInSection(list->section[i].selectors, list->section[i].attributes, selector, attributeName, stop);
        if (stop == true)break;

    }
    if (stop == false && list->previous != NULL)findSelectorInBlock(list->previous, selector, attributeName);
}

void printValueInSelInAttr(cssList* list, char* selector, char* attributeName) {
    //find last selector -> find attribute name -> store value
    int sectionAmount = 0;
    countSectionAmount(list, sectionAmount);

    list = goToblock(list, (sectionAmount - 1) / T);

    findSelectorInBlock(list, selector, attributeName);

}

//number of attributes in one section with a specific name
void countNumberAttrInOneSection(char* name, int& occurrences, attributeList* list) {
    if (list != NULL) {
        if (checkIfWordsEqual(list->name, name))occurrences++;
        if (list->next != NULL)countNumberAttrInOneSection(name, occurrences, list->next);

    }
}

void countNumberSelInOneSection(char* name, int& occurrences, selectorsList* list) {
    if (list != NULL) {
        if (checkIfWordsEqual(list->str, name))occurrences++;
        if (list->next != NULL)countNumberSelInOneSection(name, occurrences, list->next);

    }
}

void printNumberOfOccurrencesByName(cssList* list, char* name, int occurrences, char element) {
    //1 find in section
    //2 go to next section
    //3 go to next block

    if (element == 'A') {
        for (int i = 0; i < T; i++) {
            countNumberAttrInOneSection(name, occurrences, list->section[i].attributes);

        }

    }
    else if (element == 'S') {
        for (int i = 0; i < T; i++) {
            countNumberSelInOneSection(name, occurrences, list->section[i].selectors);

        }

    }

    if (list->next != NULL)printNumberOfOccurrencesByName(list->next, name, occurrences, element);
    else cout << name << "," << element << ",? == " << occurrences << endl;
}

bool commandDeleteSection(char sign1, char sign2) {
    if (sign1 == 'D' && sign2 == '*')return true;
    return false;
}
bool commandDeleteAttribute(char sign1) {
    if (sign1 == 'D')return true;
    return false;
}

void deleteNSection(cssList* list, int n, int sectionNumber) {
    int sectionIndex = 0;
    if (sectionNumber < n) {
        for (int i = 0; i < T; i++) {
            if (isSectionEmpty(list->section[i]))sectionNumber++;
            if (sectionNumber == n) {
                sectionIndex = i;
                break;

            }

        }

    }
    if (sectionNumber == n) {
        for (int i = sectionIndex; i < T; i++) {
            if (list->section[i].attributes == NULL)break;
            else {
                if (i + 1 < T) {
                    //ostatnia sekcja bloku
                    if (list->section[i + 1].attributes == NULL) {
                        list->section[i].attributes = NULL;
                        list->section[i].selectors = NULL;

                    }
                    else {
                        list->section[i].attributes = list->section[i + 1].attributes;
                        list->section[i].selectors = list->section[i + 1].selectors;

                    }

                }
                else {
                    //wez 1 element z nastepnego wezla
                    if (list->next != NULL && list->next->section[0].attributes != NULL) {
                        list->section[i].attributes = list->next->section[0].attributes;
                        list->section[i].selectors = list->next->section[0].selectors;

                    }
                    else {
                        list->section[i].attributes = NULL;
                        list->section[i].selectors = NULL;

                    }

                }

            }


        }

    }
    if (sectionNumber <= n && list->next != NULL)deleteNSection(list->next, n, sectionNumber);
}
void deleteSection(cssList* list, int num, bool print) {


    int sectionAmount = 0;
    countSectionAmount(list, sectionAmount);
    if (sectionAmount >= num) {

        deleteNSection(list, num, 0);
        //moveSections(list, num);

        if (print == true) cout << num << ",D,* == deleted" << endl;

    }

}

void moveAttributes(attributeList* attribute) {
    if (attribute != NULL) {
        if (attribute->next != NULL) {

            attribute->name = attribute->next->name;
            attribute->value = attribute->next->value;
            if (attribute->next->next != NULL) {
                attribute->next = attribute->next->next;
                attribute->next->previous = attribute;

            }
            else attribute->next = NULL;


        }

    }
}
void deleteSpecificAttribute(cssList* list, attributeList* attributes, char* attributeName, int num, int attributesAmount) {

    if (attributes != NULL && checkIfWordsEqual(attributeName, attributes->name)) {

        if (attributesAmount == 1)deleteSection(list, num, false);
        else {
            if (attributes->next != NULL && attributes->previous != NULL) {
                attributes->previous->next = attributes->next;
                attributes->next->previous = attributes->previous;
                delete attributes;

            }
            //last attribute
            else if (attributes->previous != NULL) {
                attributes->previous->next = NULL;
                delete attributes;

            }
            //first attribute
            else if (attributes->next != NULL) {

                moveAttributes(attributes);


            }


        }

        cout << num << ",D," << attributeName << " == deleted" << endl;

    }
    else if (attributes != NULL && attributes->next != NULL)deleteSpecificAttribute(list, attributes->next, attributeName, num, attributesAmount);

}

void deleteAttribute(cssList* list, int num, char* attributeName) {

    sectionList l;
    findNSection(l, list, num, 0);
    int numberOfAttributes = 0;

    attributesNumber(l.attributes, numberOfAttributes, num, false);

    deleteSpecificAttribute(list, l.attributes, attributeName, num, numberOfAttributes);
}

void recognizeCommand(char* command, cssList* list) {
    int index = getIndexOfFirstNum(command);
    int sectionAmount = 0;
    countSectionAmount(list, sectionAmount);


    if (checkIfDigit(command[0])) {
        int num = makeNumber(command, index);

        if ((command[index + 1] == 'S' || command[index + 1] == 'A') && num <= sectionAmount && command[index + 3] == '?')
            printNumberOfOccurrences(list, num, command[index + 1]);

        else if (command[index + 1] == 'A' && num <= sectionAmount) {

            char* attributeName = new char[strlen(command)];
            getAttributeName(command, attributeName, getIndexOfFirstNum(command) + 3);

            printAttributeValue(list, num, attributeName);

        }
        else if ((command[index + 1] == 'S') && checkIfDigit(command[index + 3]) && num <= sectionAmount) {

            index = getIndexOfSecondNum(command);

            int selectorNumber = makeNumber(command, index);
            printSelector(list, num, selectorNumber);

        }
        else if (commandDeleteSection(command[index + 1], command[index + 3])) {
            deleteSection(list, num, true);

        }
        else if (commandDeleteAttribute(command[index + 1])) {
            char* attributeName = new char[strlen(command)];
            getAttributeName(command, attributeName, getIndexOfFirstNum(command) + 3);
            deleteAttribute(list, num, attributeName);

        }


    }
    else {

        char* firstWord = new char[strlen(command)];

        getAttributeName(command, firstWord, 0);

        if (command[0] == '?' && command[1] == '\0') printSectionAmount(list, 0);

        else if (commandPrintOccurrenccesByName(command, index)) {
            printNumberOfOccurrencesByName(list, firstWord, 0, command[index + 1]);

        }
        else if (commandPrintValueOfSpecAttrInSpecSel(command, index)) {
            char* secondWord = new char[strlen(command)];
            getAttributeName(command, secondWord, getIndexOfFirstNum(command) + 3);

            printValueInSelInAttr(list, firstWord, secondWord);


        }


    }
}

int main()
{
    cssList* firstList = NULL;

    char str[BUFF] = {
    };
    char tmp[BUFF_TMP] = {
    };

    bool commandsSection = false;


    while (cin >> tmp && tmp[strlen(tmp) - 1] != EOF) {

        char ch = getchar();
        //jesli tmp to ???? zacznij tryb komend
        //jesli tmp to **** tryb x
        checkIfCommandsStart(tmp, commandsSection);

        if (commandsSection == false) {
            createSectionString(str, tmp);

            if (str[strlen(str) - 1] == '}') {
                createSectionAtTheEnd(str, firstList, firstList);
                clearStr(str);


            }

        }
        else {

            createCommandString(str, tmp);
            if (ch == ENTER || ch == EOF) {
                recognizeCommand(str, firstList);
                clearStr(str);

            }

        }
        if (str[strlen(str) - 1] != ',') clearStr(tmp);


    }
    freeMemory(firstList);

}