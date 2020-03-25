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

int max(int a, int b);

int height(bstNode *N);

bstNode *rightRotate(bstNode *y);

bstNode *leftRotate(bstNode *x);

bstNode *insert(bstNode *node, Date keydateValue, listNode *record);

void preOrder(bstNode *root);

int getBalance(bstNode *N);

bstNode *newNode(Date dateValue, listNode *record);

bucket **hashTableInit(int tableSize);

void hashTableInsert(bucket **hashTable, char *keyName, char* tableType, int tableSize, int bucketSize, listNode *record);

void printRecord(patientRecord record);

void printList(listNode *head);

void push(listNode **head, patientRecord **record);

bool validArgs(int argc,char *argv[]);

void getArgs(int *diseaseHashTableNumOfEntries, int *countryHashTableNumOfEntries, int *bucketSize, char **patientRecordsFile, char *argv[]);

listNode * storeData(char *patientRecordsFile);

void sortDateInsert(listNode **head,patientRecord **record);

int compareDates(listNode *current, patientRecord *record);

bool isUniqueID(listNode* head, char* newID);

int hashFunction(char *deseaseID, int hashTableSize);
