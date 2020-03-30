#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Interface.h"

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

listNode *sortDateInsert(listNode **head, patientRecord **record) {

    int sortFlag;

    if(*head == NULL) { // if list is empty
        *head = (listNode *) malloc(sizeof(listNode));
        if(*head == NULL) {
            exit(-1);
        }
        (*head)->record = *record;
        (*head)->next = NULL;
        return *head;
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
        return *head;
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
    return newNode;
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
                if(atoi(tmpDateInfo) == 0) { // if patient is still sick
                    tmpRecordPtr->exitDate.day = 0;
                    tmpRecordPtr->exitDate.month = 0;
                    tmpRecordPtr->exitDate.year = 0;
                }
                else {
                    sscanf(tmpDateInfo, "%d-%d-%d", &(tmpRecordPtr->exitDate.day), &(tmpRecordPtr->exitDate.month), &(tmpRecordPtr->exitDate.year));
                }
                if(compareStructDates(tmpRecordPtr->entryDate, tmpRecordPtr->exitDate) == 1  && tmpRecordPtr->exitDate.day != 0) {
                    printf("EntryDate at recordID = %s is newer than ExitDate! Exiting..\n", tmpRecordPtr->recordID);
                    exit(-1);
                }
                sortDateInsert(&head, &tmpRecordPtr);
                break;
        }
    }


    fclose(fp);

    return head;
}
