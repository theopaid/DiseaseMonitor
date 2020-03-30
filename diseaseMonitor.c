#include "functions.h"

int main (int argc, char *argv[]) {

    if (!validArgs(argc, argv)) {
        printf("The arguments given are invalid. Try again.\n");
        return -1;
    }

    int diseaseHashTableNumOfEntries, countryHashTableNumOfEntries, bucketSize;
    char *patientRecordsFile;
    getArgs(&diseaseHashTableNumOfEntries, &countryHashTableNumOfEntries, &bucketSize, &patientRecordsFile, argv);

    listNode *head = storeData(patientRecordsFile); // head of list with records
    printList(head);

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
        //printRecord(*(current->record));
        hashTableInsert(&diseaseHTable, current->record->diseaseID, current);
        hashTableInsert(&countryHTable, current->record->country, current);
        current = current->next;
    }
    //printf("last: %s\n", diseaseHashTable[2]->next->pairsInBucket[0].root->record->next->record->recordID);
    //preOrder(diseaseHashTable[7]->pairsInBucket[0].root);

    //printf("last: %s\n", countryHashTable[3]->pairsInBucket[1].root->right->record->record->recordID);

    renderMenu(&diseaseHTable, &countryHTable, head);

    return 0;
}
