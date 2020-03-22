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

    listNode *current = head;
    while (current != NULL) {
        //printRecord(*(current->record));
        hashTableInsert(diseaseHashTable, current->record->diseaseID, "disease", diseaseHashTableNumOfEntries, bucketSize, current);
        current = current->next;
    }

    char* test = "Cdgjlkbnsdojnbg";
    printf("hashResult: %d\n", hashFunction(test, diseaseHashTableNumOfEntries));

    return 0;
}
