#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"


void printRecord(patientRecord record) {

    printf("%s | %s | %s | %s | %s | %s | %s\n", record.recordID, record.patientFirstName, record.patientLastName, record.diseaseID, record.country, record.entryDate, record.exitDate);
}

void printList(listNode *head) {

    listNode *current = head;
    while (current != NULL) {
        printRecord(*(current->record));
        current = current->next;
    }
    
}

void push(listNode **head, patientRecord **record) {

    if(*head == NULL) { // list empty
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

listNode * storeData(char *patientRecordsFile) {

    listNode *head = NULL;

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
                fscanf(fp, "%s", tmpRecordPtr->recordID);
                break;
            case 1:
                fscanf(fp, "%s", tmpRecordPtr->patientFirstName);
                break;
            case 2:
                fscanf(fp, "%s", tmpRecordPtr->patientLastName);
                break;
            case 3:
                fscanf(fp, "%s", tmpRecordPtr->diseaseID);
                break;
            case 4:
                fscanf(fp, "%s", tmpRecordPtr->country);
                break;
            case 5:
                fscanf(fp, "%s", tmpRecordPtr->entryDate);
                break;
            case 6: // last entry of a record
                fscanf(fp, "%s", tmpRecordPtr->exitDate);
                push(&head, &tmpRecordPtr);
                break;
        }
    }


    fclose(fp);

    return head;
}