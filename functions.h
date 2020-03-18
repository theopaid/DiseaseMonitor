#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1;
#define false 0;

typedef struct Date
{
    int day;
    int month;
    int year;
} Date;


typedef struct patientRecord
{
    char *recordID;
    char *patientFirstName;
    char *patientLastName;
    char *diseaseID;
    char *country;
    Date entryDate;
    Date exitDate;
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

void sortDateInsert(listNode **head,patientRecord **record);

int compareDates(listNode *current, patientRecord *record);

bool isUniqueID(listNode* head, char* newID);
