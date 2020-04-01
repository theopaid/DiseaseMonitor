#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Interface.h"

void renderMenu(hashTable *diseaseHTable, hashTable *countryHTable, listNode *head, uniqueNamesList *headOfUniqueCountries, uniqueNamesList *headOfUniqueDiseases)
{

    char line[LINE_MAX];
    char *command = NULL, *userInput = NULL, *arguments = NULL;

    printf("-------------------------------------------------------------------\n\n");
    printf("\t\t----------- DISEASE MONITOR -----------\n\n");
    printf("For instructions on how to use the app, type /man :\n");

    while(1) {

        printf("\n%s> ", "DiseaseMonitor");

        if(fgets(line, LINE_MAX, stdin) == NULL) {
            break;
        }

        userInput = line;
        command = strtok_r(userInput, " \n", &userInput); // Getting main
        arguments = strtok(userInput, "\n"); // Getting all the arguments. NULL if there are none

        if(command != NULL) {
            if(strcmp(command, "/globalDiseaseStats") == 0) {
                if(arguments != NULL && strlen(arguments) < 19) {
                    printf("Time specific search needs both Entry and Exit dates!\n");
                    continue;
                }
                else {
                    globalDiseaseStats(arguments, diseaseHTable);
                }
            }
            else if(strcmp(command, "/diseaseFrequency") == 0) {
                if(arguments != NULL && strlen(arguments) < 10) {
                    printf("Data is not valid!\n");
                    continue;
                }
                else if(arguments == NULL) {
                    printf("No arguments were given!\n");
                }
                else {
                    diseaseFrequency(arguments, diseaseHTable);
                }
            }
            else if(strcmp(command, "/topk-Diseases") == 0) {
                if(arguments != NULL && (strlen(arguments) < 24 && strlen(arguments) > 12)) {
                    printf("Time specific search needs both Entry and Exit dates!\n");
                    continue;
                }
                else {
                    //topDiseases(arguments, diseaseHTable, head);
                }
            }
            else if(strcmp(command, "/topk-Countries") == 0) {
                if(arguments != NULL && (strlen(arguments) < 24 && strlen(arguments) > 15)) {
                    printf("Time specific search needs both Entry and Exit dates!\n");
                    continue;
                }
                else {
                    //topCountries(arguments, countryHTable, head);
                }
            }
            else if(strcmp(command, "/insertPatientRecord") == 0) {
                if(arguments != NULL && strlen(arguments) < 30) {
                    printf("Data is not valid!\n");
                    continue;
                }
                else {
                    insertPatientRecord(arguments, diseaseHTable, countryHTable, head, headOfUniqueCountries, headOfUniqueDiseases);
                }
            }
            else if(strcmp(command, "/recordPatientExit") == 0) {
                if(arguments != NULL && strlen(arguments) < 10) {
                    printf("Enter a valid Exit date!\n");
                    continue;
                }
                else {
                    recordPatientExit(arguments, head);
                }
            }
            else if(strcmp(command, "/numCurrentPatients") == 0) {
                numCurrentPatients(arguments, diseaseHTable);
            }
            else if(strcmp(command, "/man") == 0) {
                printManual();
            }
            else if(strcmp(command, "/exit") == 0) {
                printf("\nExiting the application. Goodbye and stay safe..\n");
                return;
            }
            else {
                printf("Command not found!\n");
            }
        }
    }
}

void globalDiseaseStats(char* arguments,hashTable *diseaseHTable) {

    int htSize = diseaseHTable->counter;

    if(arguments != NULL) { // time specific search

        Date entryDate, exitDate;
        inputToDates(arguments, &entryDate, &exitDate);

        if(compareStructDates(entryDate, exitDate) == 1) { // entryDate > exitDate
            printf("Entry date must be earlier than Exit date!\n");
            return;
        }
        for(int i=0; i < htSize; i++) {
            if(diseaseHTable->bucketPtrs[i] != NULL) { // if there is data in this hashNode

                bucket *currentBucket = diseaseHTable->bucketPtrs[i];
                while(currentBucket != NULL) { // iterating all the buckets

                    for(int j=0; j < currentBucket->pairsCounter; j++) { // accessing all keys of a bucket

                        printf("%s %d\n", currentBucket->pairsInBucket[j].key, preOrderCounterWDates(currentBucket->pairsInBucket[j].root, entryDate, exitDate));
                    }

                    currentBucket = currentBucket->next;
                }
            }
        }
    }
    else {

        for(int i=0; i < htSize; i++) {
            if(diseaseHTable->bucketPtrs[i] != NULL) { // if there is data in this hashNode
                bucket *currentBucket = diseaseHTable->bucketPtrs[i];
                while(currentBucket != NULL) { // iterating all the buckets

                    for(int j=0; j < currentBucket->pairsCounter; j++) { // accessing all keys of a bucket
                        printf("%s %d\n", currentBucket->pairsInBucket[j].key, preOrderCounter(currentBucket->pairsInBucket[j].root));
                    }

                    currentBucket = currentBucket->next;
                }
            }
        }
    }
}

void diseaseFrequency(char *arguments,hashTable *diseaseHTable) {

    int htPos;
    char *localArgs;
    char *virusName, *country, *entryDateStr, *exitDateStr;
    int count = 0;
    bucket *currentBucket;

    localArgs = strtok(arguments, " ");
    while(localArgs != NULL) {
        count++;
        switch (count) {
            case 1:
                virusName = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(virusName, localArgs);
                break;
            case 2:
                entryDateStr = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(entryDateStr, localArgs);
                break;
            case 3:
                exitDateStr = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(exitDateStr, localArgs);
                break;
            case 4:
                country = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(country, localArgs);
                break;
        }
        localArgs = strtok(NULL, " ");
    }
    // if(count == 3) { // if a country was not given as an argument
    //     // replace the date values
    //     exitDateStr = malloc(sizeof(char) * (strlen(entryDateStr) + 1));
    //     strcpy(exitDateStr, entryDateStr);
    //     strcpy(entryDateStr, country);
    //     free(country);
    //     country = NULL;
    // }

    if(count == 3) { // we need to set country to NULL;
        country = NULL;
    }

    htPos = hashFunction(virusName, diseaseHTable->counter);

    Date entryDate, exitDate;
    inputToDate(entryDateStr, &entryDate);
    inputToDate(exitDateStr, &exitDate);
    if(compareStructDates(entryDate, exitDate) == 1) { // entryDate > exitDate
        printf("Entry date must be earlier than Exit date!\n");
        return;
    }

    currentBucket = diseaseHTable->bucketPtrs[htPos];
    while(currentBucket != NULL) { // iterating all the buckets

        for(int j=0; j < currentBucket->pairsCounter; j++) { // accessing all keys of a bucket
            if(strcmp(currentBucket->pairsInBucket[j].key, virusName) == 0) {
                printf("%s %d\n", virusName, preOrderDiseaseCountryCounter(currentBucket->pairsInBucket[j].root, country, entryDate, exitDate));
                return;
            }
        }

        currentBucket = currentBucket->next;
    }
    printf("There is no Disease named that way!\n");
    return;
}

void insertPatientRecord(char *arguments, hashTable *diseaseHTable, hashTable *countryHTable, listNode *head, uniqueNamesList *headOfUniqueCountries, uniqueNamesList *headOfUniqueDiseases)
{

    char *localArgs;
    char *virusName, *country, *entryDateStr, *exitDateStr, *recordID, *firstName, *lastName;
    int count = 0;
    bucket *currentBucket;
    patientRecord *tmpRecordPtr;
    tmpRecordPtr = malloc(sizeof(patientRecord));
    if(tmpRecordPtr == NULL) {
        freeList(head);
        freeHTable(diseaseHTable);
        freeHTable(countryHTable);
        exit(-1);
    }

    localArgs = strtok(arguments, " ");
    while(localArgs != NULL) {
        count++;
        switch (count) {
            case 1:
                tmpRecordPtr->recordID = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(tmpRecordPtr->recordID, localArgs);
                break;
            case 2:
                tmpRecordPtr->patientFirstName = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(tmpRecordPtr->patientFirstName, localArgs);
                break;
            case 3:
                tmpRecordPtr->patientLastName = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(tmpRecordPtr->patientLastName, localArgs);
                break;
            case 4:
                tmpRecordPtr->diseaseID = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(tmpRecordPtr->diseaseID, localArgs);
                break;
            case 5:
                tmpRecordPtr->country = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(tmpRecordPtr->country, localArgs);
                break;
            case 6:
                entryDateStr = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(entryDateStr, localArgs);
                break;
            case 7:
                exitDateStr = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(exitDateStr, localArgs);
                break;
        }
        localArgs = strtok(NULL, " ");
    }

    //Date entryDate, exitDate;
    if(count == 6) { // we need to set exitDate to 0-0-0;
        tmpRecordPtr->exitDate.day = 0;
        tmpRecordPtr->exitDate.month = 0;
        tmpRecordPtr->exitDate.year = 0;
        inputToDate(entryDateStr, &(tmpRecordPtr->entryDate));
    }
    else {
        inputToDate(entryDateStr, &(tmpRecordPtr->entryDate));
        inputToDate(exitDateStr, &(tmpRecordPtr->exitDate));
        if(compareStructDates(tmpRecordPtr->entryDate, tmpRecordPtr->exitDate) == 1) { // entryDate > exitDate
            printf("Entry date must be earlier than Exit date!\n");
            return;
        }
    }

    if(!isUniqueID(head, tmpRecordPtr->recordID)) {
        printf("Multiple instances of the same ID found! Operation shuts down.\n");
        return;
    }

    listNode *insertedNode = sortDateInsert(&head, &tmpRecordPtr);
    insertUnique(&headOfUniqueCountries, tmpRecordPtr->country);
    insertUnique(&headOfUniqueDiseases, tmpRecordPtr->diseaseID);
    hashTableInsert(diseaseHTable, insertedNode->record->diseaseID, insertedNode);
    hashTableInsert(countryHTable, insertedNode->record->country, insertedNode);
    printUniqueList(headOfUniqueCountries);
    printUniqueList(headOfUniqueDiseases);

    printf("Record added\n");
}

void recordPatientExit(char *arguments,listNode *head) {

    char *localArgs;
    char *recordID, *exitDateStr;
    int count = 0;

    localArgs = strtok(arguments, " ");
    while(localArgs != NULL) {
        count++;
        switch (count) {
            case 1:
                recordID = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(recordID, localArgs);
                break;
            case 2:
                exitDateStr = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(exitDateStr, localArgs);
                break;
        }
        localArgs = strtok(NULL, " ");
    }

    Date exitDate;
    inputToDate(exitDateStr, &exitDate);

    listNode *current = head;
    while(current != NULL) {
        if(strcmp(current->record->recordID, recordID) == 0) { // if a record with the same ID is found
            if(compareStructDates(current->record->entryDate, exitDate) == 1) {
                printf("The Exit date should be newer than the Entry!\n");
                return;
            }
            else {
                current->record->exitDate.day = exitDate.day;
                current->record->exitDate.month = exitDate.month;
                current->record->exitDate.year = exitDate.year;
                printf("exiting\n");
                return;
            }
        }

        current = current->next;
    }
    printf("No records found with the recordID given!\n");
}

void numCurrentPatients(char *arguments, hashTable *diseaseHTable) {

    char *localArgs;
    char *virusName;
    int count = 0, htPos = 0;
    bucket *currentBucket;

    localArgs = strtok(arguments, " ");
    while(localArgs != NULL) {
        count++;
        switch (count) {
            case 1:
                virusName = malloc(sizeof(char) * (strlen(localArgs) + 1));
                strcpy(virusName, localArgs);
                break;
        }
        localArgs = strtok(NULL, " ");
    }

    if(count == 0) { // we need to set virusName to NULL;
        virusName = NULL;
    }
    else {
        htPos = hashFunction(virusName, diseaseHTable->counter);
    }

    if(virusName != NULL) {

        currentBucket = diseaseHTable->bucketPtrs[htPos];
        while(currentBucket != NULL) { // iterating all the buckets

            for(int j=0; j < currentBucket->pairsCounter; j++) { // accessing all keys of a bucket
                if(strcmp(currentBucket->pairsInBucket[j].key, virusName) == 0) {
                    printf("%s %d\n", virusName, preOrderDiseaseCounterWDates(currentBucket->pairsInBucket[j].root));
                    return;
                }
            }

            currentBucket = currentBucket->next;
        }

        printf("There is no Disease named that way!\n");
        return;
    }
    else {

        int htSize = diseaseHTable->counter;

        for(int i=0; i < htSize; i++) {
            if(diseaseHTable->bucketPtrs[i] != NULL) { // if there is data in this hashNode
                currentBucket = diseaseHTable->bucketPtrs[i];
                while(currentBucket != NULL) { // iterating all the buckets

                    for(int j=0; j < currentBucket->pairsCounter; j++) { // accessing all keys of a bucket
                        prerOrderPrinterWDates(currentBucket->pairsInBucket[j].root);
                    }

                    currentBucket = currentBucket->next;
                }
            }
        }
    }


}

void printManual() {

    char word;
    FILE *fp = fopen("bin/manual.txt", "r");

    if(fp == NULL) {
        printf("Could not open file manual.txt\n");
        exit(-1);
    }

    while(!feof(fp)) {
        fscanf(fp, "%c", &word);
        printf("%c", word);
    }
}
