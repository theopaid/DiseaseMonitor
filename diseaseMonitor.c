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
    bucket **diseaseHashTable = hashTableInit(diseaseHashTableNumOfEntries);
    bucket **countryHashTable = hashTableInit(countryHashTableNumOfEntries);

    listNode *current = head;
    while (current != NULL) {
        //printRecord(*(current->record));
        hashTableInsert(diseaseHashTable, current->record->diseaseID, "disease", diseaseHashTableNumOfEntries, bucketSize, current);
        hashTableInsert(countryHashTable, current->record->country, "country", countryHashTableNumOfEntries, bucketSize, current);
        current = current->next;
    }
    printf("last: %s\n", diseaseHashTable[2]->next->pairsInBucket[0].root->record->next->record->recordID);
    preOrder(diseaseHashTable[7]->pairsInBucket[0].root);

    printf("last: %s\n", countryHashTable[3]->pairsInBucket[0].root->record->next->record->recordID);

    //renderMenu();

    return 0;
}
