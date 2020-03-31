#include "../include/Interface.h"

int main (int argc, char *argv[]) {

    if (!validArgs(argc, argv)) {
        printf("The arguments given are invalid. Try again.\n");
        return -1;
    }

    int diseaseHashTableNumOfEntries, countryHashTableNumOfEntries, bucketSize;
    char *patientRecordsFile;
    getArgs(&diseaseHashTableNumOfEntries, &countryHashTableNumOfEntries, &bucketSize, &patientRecordsFile, argv);

    listNode *head = storeData(patientRecordsFile); // head of list with records
    //printList(head);

    // lest create the hash tables
    hashTable diseaseHTable;
    bucket **diseaseHashTable = hashTableInit(diseaseHashTableNumOfEntries);
    diseaseHTable.bucketPtrs = diseaseHashTable;
    diseaseHTable.counter = diseaseHashTableNumOfEntries;
    diseaseHTable.bucketSize = bucketSize;

    hashTable countryHTable;
    bucket **countryHashTable = hashTableInit(countryHashTableNumOfEntries);
    countryHTable.bucketPtrs = countryHashTable;
    countryHTable.counter = countryHashTableNumOfEntries;
    countryHTable.bucketSize = bucketSize;

    listNode *current = head;
    while (current != NULL) {
        hashTableInsert(&diseaseHTable, current->record->diseaseID, current);
        hashTableInsert(&countryHTable, current->record->country, current);
        current = current->next;
    }

    renderMenu(&diseaseHTable, &countryHTable, head);

    freeList(head);
    freeHTable(&diseaseHTable);
    freeHTable(&countryHTable);


    return 0;
}
