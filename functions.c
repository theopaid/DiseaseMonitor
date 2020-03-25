#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"


bucket **hashTableInit(int tableSize) {

    bucket **hashTable = malloc(tableSize * sizeof(bucket*));
    int i;
    for(i = 0; i < tableSize; i++) {
        hashTable[i] = NULL;
    }
    return hashTable;
}

void hashTableInsert(bucket **hashTable, char *keyName, char* tableType, int tableSize, int bucketSize, listNode *record) {

    int key = hashFunction(keyName, tableSize);
    int remainingSize = bucketSize - sizeof(int) - sizeof(bucket*);
    int canHoldPairs = remainingSize / (sizeof(char*) + sizeof(bstNode*));
    int i;
    printf("lets insert/ at/pairs: %s | %d | %d\n", keyName, key, canHoldPairs);

    if(canHoldPairs < 1) {
        printf("Can't hold any info, because the bucket size given is pretty small!\n");
        exit(-1);
    }

    if(hashTable[key] == NULL) {
        printf("entered_1\n");
        // if this index is empty create new bucket and allocate memory for it
        hashTable[key] = malloc(sizeof(bucket));
        hashTable[key]->next = NULL;
        hashTable[key]->pairsInBucket = malloc(canHoldPairs * sizeof(bucketPair));
        hashTable[key]->pairsInBucket[0].root = NULL;
        hashTable[key]->pairsInBucket[0].key = malloc(sizeof(char)*(strlen(keyName) + 1));
        strcpy(hashTable[key]->pairsInBucket[0].key, keyName);
        hashTable[key]->pairsInBucket[0].root = insert(hashTable[key]->pairsInBucket[0].root, record->record->entryDate, record);
        printf("1: \n");
        preOrder(hashTable[key]->pairsInBucket[0].root);
        hashTable[key]->pairsCounter = 1;
        printf("inserted at: %d\n", key);
    }
    else {
        printf("entered_mid\n");
        bucket *tempBucket = hashTable[key];
        while(1) {
            printf("entered_mid2\n");
            for(i = 0; i < tempBucket->pairsCounter; i++) {
                if(strcmp(keyName, tempBucket->pairsInBucket[i].key) == 0) {
                    printf("already stored in pos: %d\n", i);
                    // this key is already stored in a bucket
                    hashTable[key]->pairsInBucket[0].root = insert(hashTable[key]->pairsInBucket[0].root, record->record->entryDate, record);
                    printf("2: \n");
                    preOrder(hashTable[key]->pairsInBucket[0].root);
                    return;
                }
            }
            if(tempBucket->next == NULL) {
                break;
            }
            tempBucket = tempBucket->next;
        }
        // first check if there is space in the last bucket
        if(tempBucket->pairsCounter < canHoldPairs) {
            printf("entered_2\n");
            // there is free space
            printf("ok1\n");
            tempBucket->pairsInBucket[tempBucket->pairsCounter].key = malloc(sizeof(char)*(strlen(keyName) + 1));
            strcpy(tempBucket->pairsInBucket[tempBucket->pairsCounter].key, keyName);
            tempBucket->pairsInBucket[tempBucket->pairsCounter].root = insert(tempBucket->pairsInBucket[tempBucket->pairsCounter].root, record->record->entryDate, record);
            printf("3: \n");
            preOrder(tempBucket->pairsInBucket[tempBucket->pairsCounter].root);
            printf("ok2\n");
            tempBucket->pairsCounter += 1;
            printf("inserted_2 at: %d\n", key);
            return;
        }
        // we need to create a new bucket
        printf("entered_3\n");
        tempBucket->next = malloc(sizeof(bucket));
        tempBucket = tempBucket->next; // point to newly created bucket
        tempBucket->next = NULL;
        tempBucket->pairsInBucket = malloc(canHoldPairs * sizeof(bucketPair));
        tempBucket->pairsInBucket[0].root = NULL;

        tempBucket->pairsInBucket[0].key  = malloc(sizeof(char)*(strlen(keyName) + 1));
        strcpy(tempBucket->pairsInBucket[0].key, keyName);
        tempBucket->pairsCounter = 1;
        tempBucket->pairsInBucket[0].root = insert(tempBucket->pairsInBucket[0].root, record->record->entryDate, record);
        printf("4: \n");
        preOrder(tempBucket->pairsInBucket[0].root);
        printf("inserted_3 at: %d\n", key);

    }
}

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

int hashFunction(char *keyName, int hashTableSize) {

    int hashVal = 0;
    int len = strlen(keyName);

    for(int i = 0; i < len; i++) {
        hashVal += keyName[i];
    }

    return hashVal % hashTableSize;
}

bstNode *newNode(Date keydateValue, listNode *record) {

    bstNode *node = (bstNode *)malloc(sizeof(bstNode));
    node->record = record;
    node->dateValue = keydateValue;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->count = 1;
    return (node);
}

bstNode *rightRotate(bstNode *y) {

    bstNode *x = y->left;
    bstNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

bstNode *leftRotate(bstNode *x) {

    bstNode *y = x->right;
    bstNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(bstNode *N) {

    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

bstNode *insert(bstNode *node, Date keydateValue, listNode *record) {

    if (node == NULL) {
        return (newNode(keydateValue, record));
    }

    Date nodesDate = node->dateValue;

    if (nodesDate.year==keydateValue.year && nodesDate.month==keydateValue.month && nodesDate.day==keydateValue.day) {
        (node->count)++;
        return node;
    }

    if (compareStructDates(keydateValue, nodesDate) == -1) {
        node->left = insert(node->left, keydateValue, record);
    }
    else {
        node->right = insert(node->right, keydateValue, record);
    }

    node->height = max(height(node->left), height(node->right)) + 1;

    int balance = getBalance(node);

    if(balance > 1 && compareStructDates(keydateValue, node->left->dateValue) == -1) {
        return rightRotate(node);
    }

    if(balance < -1 && compareStructDates(keydateValue, node->right->dateValue) == 1) {
        return leftRotate(node);
    }

    if(balance > 1 && compareStructDates(keydateValue, node->left->dateValue) == 1) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if(balance < -1 && compareStructDates(keydateValue, node->right->dateValue) == -1) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

int max(int a, int b) {

    return (a > b) ? a : b;
}

int height(bstNode *N) {

    if(N == NULL) {
        return 0;
    }
    return N->height;
}

void preOrder(bstNode *root) {

    if (root != NULL) {
        printf("%d(%d) ", root->dateValue.day, root->count);
        preOrder(root->left);
        preOrder(root->right);
    }
}
