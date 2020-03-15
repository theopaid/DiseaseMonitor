#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1;
#define false 0;

typedef struct patientRecord
{
    char recordID[32];
    char patientFirstName[32];
    char patientLastName[32];
    char diseaseID[32];
    char country[32];
    char entryDate[11];
    char exitDate[11];
} patientRecord;

typedef struct listNode
{
    patientRecord *record;
    struct listNode *next;
} listNode;

void printRecord(patientRecord record);

void printList(listNode *head);

void push(listNode **head, patientRecord **record);

bool validArgs(int argc,char *argv[]);

void getArgs(int *diseaseHashTableNumOfEntries, int *countryHashTableNumOfEntries, int *bucketSize, char **patientRecordsFile, char *argv[]);

listNode * storeData(char *patientRecordsFile);
