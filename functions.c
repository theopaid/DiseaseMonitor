#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"


void printRecord(patientRecord record) {

    printf("%s | %s | %s | %s | %s | %d-%d-%d | %d-%d-%d\n", record.recordID, record.patientFirstName, record.patientLastName, record.diseaseID, record.country, record.entryDate.day, record.entryDate.month, record.entryDate.year, record.exitDate.day, record.exitDate.month, record.exitDate.year);
}

void printList(listNode *head) {

    listNode *current = head;
    while (current != NULL) {
        printRecord(*(current->record));
        current = current->next;
    }
    
}

void push(listNode **head, patientRecord **record) {

    if(*head == NULL) { // if list is empty
        *head = (listNode *) malloc(sizeof(listNode));
        if(*head == NULL) {
            exit(-1);
        }
        (*head)->record = *record;
        (*head)->next = NULL;
        return;
    }

    listNode *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (listNode *) malloc(sizeof(listNode));
    if(current->next == NULL) {
        exit(-1);
    }
    current->next->record = *record;
    current->next->next = NULL;    
}

void sortDateInsert(listNode **head, patientRecord **record) {

    int sortFlag;

    if(*head == NULL) { // if list is empty
        *head = (listNode *) malloc(sizeof(listNode));
        if(*head == NULL) {
            exit(-1);
        }
        (*head)->record = *record;
        (*head)->next = NULL;
        return;
    }

    listNode *current;
    sortFlag = compareDates(*head, *record);

    if(sortFlag == 0 || sortFlag == 1) { // if new record's date is older than head's (or same)
        current = (listNode *) malloc(sizeof(listNode));
        if(current == NULL) {
            exit(-1);
        }
        current->record = *record;
        current->next = *head;
        *head = current;
        return;
    }

    current = *head;
    listNode *previous = NULL;
    listNode *newNode = (listNode *) malloc(sizeof(listNode));
        if(newNode == NULL) {
            exit(-1);
        }
    newNode->record = *record;

    while(current->next != NULL && compareDates(current, *record) == 2) {
        previous = current;
        current = current->next;
    }

    if(current->next == NULL) {
        if(compareDates(current, *record) == 2) {
            current->next = newNode;
            newNode->next = NULL;
        }
        else {
            newNode->next = current;
            previous->next = newNode;
        }
    }
    else {
        newNode->next = current;
        previous->next = newNode;
    }

}

int compareDates(listNode *current, patientRecord *record) { // 0:for same dates
                                                             // 1:if new Date is older than list node's one
    int sameDates = 0;                                       // 2:if new Date is newer than listNode's one

    if(record->entryDate.year < current->record->entryDate.year) {
        return 1;
    }
    else if(record->entryDate.year > current->record->entryDate.year) {
        return 2;
    }

    if(record->entryDate.month < current->record->entryDate.month) {
        return 1;
    }
    else if(record->entryDate.month > current->record->entryDate.month) {
        return 2;
    }

    if(record->entryDate.day < current->record->entryDate.day) {
        return 1;
    }
    else if(record->entryDate.day > current->record->entryDate.day) {
        return 2;
    }

    return sameDates;
}

bool validArgs(int argc,char *argv[]) {

    if (argc == 9) {
        return true;
    }
    else {
        return false;
    }
}

void getArgs(int *diseaseHashTableNumOfEntries, int *countryHashTableNumOfEntries, int *bucketSize, char **patientRecordsFile, char *argv[]) {

    for(int i = 1; i < 9; i = i + 2) {
        if(!strcmp(argv[i], "-p")) {
            *patientRecordsFile = argv[i+1];
        }
        else if(!strcmp(argv[i], "-h1")) {
            *diseaseHashTableNumOfEntries = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-h2")) {
            *countryHashTableNumOfEntries = atoi(argv[i+1]);
        }
        else if(!strcmp(argv[i], "-b")) {
            *bucketSize = atoi(argv[i+1]);
        }
    }
}

bool isUniqueID(listNode *head, char *newID) {

    listNode *current = head;

    while(current->next != NULL && strcmp(current->record->recordID, newID)) {
        current = current->next;
    }

    if(current->next == NULL) {
        if(strcmp(current->record->recordID, newID)) {
            return true;
        }
    }

    return false;
}

listNode * storeData(char *patientRecordsFile) {

    listNode *head = NULL;
    char tmpDateInfo[11];
    char tmpEntryInfo[32];

    FILE *fp = fopen(patientRecordsFile, "r");

    if(fp == NULL) {
        printf("Could not open file %s\n", patientRecordsFile);
        exit(-1);
    }

    patientRecord *tmpRecordPtr;

    for(int i = 0; !feof(fp); i++) {

        switch(i % 7)
        {
            case 0: // start of a record
                tmpRecordPtr = malloc(sizeof(patientRecord));
                if(tmpRecordPtr == NULL) {
                    exit(-1);
                }

                fscanf(fp, "%s", tmpEntryInfo);

                if(head != NULL) {
                    if(!isUniqueID(head, tmpEntryInfo)) {
                        printf("Multiple instances of the same ID found! Operation shuts down.\n");
                        //programExit();
                        exit(-1);
                    }
                }

                tmpRecordPtr->recordID = malloc(sizeof(char) * (strlen(tmpEntryInfo) + 1));
                strcpy(tmpRecordPtr->recordID, tmpEntryInfo);

                break;
            case 1:
                fscanf(fp, "%s", tmpEntryInfo);
                tmpRecordPtr->patientFirstName = malloc(sizeof(char) * (strlen(tmpEntryInfo) + 1));
                strcpy(tmpRecordPtr->patientFirstName, tmpEntryInfo);

                break;
            case 2:
                fscanf(fp, "%s", tmpEntryInfo);
                tmpRecordPtr->patientLastName = malloc(sizeof(char) * (strlen(tmpEntryInfo) + 1));
                strcpy(tmpRecordPtr->patientLastName, tmpEntryInfo);

                break;
            case 3:
                fscanf(fp, "%s", tmpEntryInfo);
                tmpRecordPtr->diseaseID = malloc(sizeof(char) * (strlen(tmpEntryInfo) + 1));
                strcpy(tmpRecordPtr->diseaseID, tmpEntryInfo);

                break;
            case 4:
                fscanf(fp, "%s", tmpEntryInfo);
                tmpRecordPtr->country = malloc(sizeof(char) * (strlen(tmpEntryInfo) + 1));
                strcpy(tmpRecordPtr->country, tmpEntryInfo);

                break;
            case 5:
                fscanf(fp, "%s", tmpDateInfo);
                sscanf(tmpDateInfo, "%d-%d-%d", &(tmpRecordPtr->entryDate.day), &(tmpRecordPtr->entryDate.month), &(tmpRecordPtr->entryDate.year));
                break;
            case 6: // last entry of a record
                fscanf(fp, "%s", tmpDateInfo);
                sscanf(tmpDateInfo, "%d-%d-%d", &(tmpRecordPtr->exitDate.day), &(tmpRecordPtr->exitDate.month), &(tmpRecordPtr->exitDate.year));
                sortDateInsert(&head, &tmpRecordPtr);
                break;
        }
    }


    fclose(fp);

    return head;
}