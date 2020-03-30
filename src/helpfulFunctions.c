#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Interface.h"

void printRecord(patientRecord record) {

    printf("%s | %s | %s | %s | %s | %d-%d-%d | %d-%d-%d\n", record.recordID, record.patientFirstName, record.patientLastName, record.diseaseID, record.country, record.entryDate.day, record.entryDate.month, record.entryDate.year, record.exitDate.day, record.exitDate.month, record.exitDate.year);
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

int compareStructDates(Date date1, Date date2) {             // -1: date1 < date2
                                                             // 0: date1 = date2
    int sameDates = 0;                                       // 1: date1 > date2

    if(date1.year < date2.year) {
        return -1;
    }
    else if(date1.year > date2.year) {
        return 1;
    }

    if(date1.month < date2.month) {
        return -1;
    }
    else if(date1.month > date2.month) {
        return 1;
    }

    if(date1.day < date2.day) {
        return -1;
    }
    else if(date1.day > date2.day) {
        return 1;
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

int hashFunction(char *keyName, int hashTableSize) {

    int hashVal = 0;
    int len = strlen(keyName);

    for(int i = 0; i < len; i++) {
        hashVal += keyName[i];
    }

    return hashVal % hashTableSize;
}

void inputToDates(char *arguments, Date *entryDate, Date *exitDate) {

    sscanf(arguments, "%d-%d-%d %d-%d-%d", &(entryDate->day), &(entryDate->month), &(entryDate->year), &(exitDate->day), &(exitDate->month), &(exitDate->year));
}

void inputToDate(char *argument, Date *date) {

    sscanf(argument, "%d-%d-%d", &(date->day), &(date->month), &(date->year));
}
