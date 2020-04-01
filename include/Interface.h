#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 1000

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

typedef struct uniqueNamesList
{
    char *nameId;
    int counter;
    struct uniqueNamesList *next;
} uniqueNamesList;

typedef struct bstNode
{
    listNode *record;
    Date dateValue;
    struct bstNode *left;
    struct bstNode *right;
    int height;
    int count;
} bstNode;

typedef struct bucketPair
{
    char *key;
    bstNode *root;
} bucketPair;

typedef struct bucket
{
    bucketPair *pairsInBucket;
    int pairsCounter;
    struct bucket *next;
} bucket;

typedef struct hashTable
{
    bucket **bucketPtrs;
    int counter;
    int bucketSize;
} hashTable;

void renderMenu(hashTable *diseaseHTable, hashTable *countryHTable, listNode *head, uniqueNamesList *headOfUniqueCountries, uniqueNamesList *headOfUniqueDiseases);

void printManual();

void inputToDates(char *arguments, Date *entryDate, Date *exitDate);

void inputToDate(char *argument, Date *date);

void globalDiseaseStats(char* arguments, hashTable *diseaseHTable);

void diseaseFrequency(char *arguments, hashTable *diseaseHashTable);

void insertPatientRecord(char *arguments, hashTable *diseaseHTable, hashTable *countryHTable, listNode *head, uniqueNamesList *headOfUniqueCountries, uniqueNamesList *headOfUniqueDiseases);

void recordPatientExit(char *arguments,listNode *head);

void numCurrentPatients(char *arguments, hashTable *diseaseHTable);

int max(int a, int b);

int height(bstNode *N);

bstNode *rightRotate(bstNode *y);

bstNode *leftRotate(bstNode *x);

bstNode *insert(bstNode *node, Date keydateValue, listNode *record);

void preOrder(bstNode *root);

int preOrderCounter(bstNode *root);

int preOrderCounterWDates(bstNode *root, Date entryDate, Date exitDate);

int preOrderDiseaseCountryCounter(bstNode *root, char *country, Date entryDate, Date exitDate);

int preOrderDiseaseCounterWDates(bstNode *root);

void prerOrderPrinterWDates(bstNode *root);

int getBalance(bstNode *N);

bstNode *newNode(Date dateValue, listNode *record);

bucket **hashTableInit(int tableSize);

void hashTableInsert(hashTable *hTable, char *keyName, listNode *record);

void printRecord(patientRecord record);

void printList(listNode *head);

void push(listNode **head, patientRecord **record);

bool validArgs(int argc,char *argv[]);

void getArgs(int *diseaseHashTableNumOfEntries, int *countryHashTableNumOfEntries, int *bucketSize, char **patientRecordsFile, char *argv[]);

listNode * storeData(char *patientRecordsFile, uniqueNamesList **headOfUniqueCountries, uniqueNamesList **headOfUniqueDiseases);

listNode *sortDateInsert(listNode **head,patientRecord **record);

int compareDates(listNode *current, patientRecord *record);

int compareStructDates(Date date1, Date date2);

bool isUniqueID(listNode* head, char* newID);

int hashFunction(char *deseaseID, int hashTableSize);

void freeList(listNode *head);

void freeAVL(bstNode *root);

void freeBuckets(bucket *firstBucket);

void freeHTable(hashTable *hashTable);

void printUniqueList(uniqueNamesList *head);

void insertUnique(uniqueNamesList **head, char *name);
