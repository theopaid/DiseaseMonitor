#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1;
#define false 0;

struct patientRecord
{
    char recordID[32];
    char patientFirstName[32];
    char patientLastName[32];
    char diseaseID[32];
    char country[32];
    char entryDate[11];
    char exitDate[11];
};

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

void storeData(char *patientRecordsFile) {

    char data[21][32];
    FILE *fp = fopen(patientRecordsFile, "r");

    if(fp == NULL) {
        printf("Could not open file %s\n", patientRecordsFile);
        exit(-1);
    }

    struct patientRecord *tmpRecordPtr;

    for(int i = 0; !feof(fp); i++) {
        fscanf(fp, "%s", data[i]);

        switch(i % 7)
        {
            case 0: // start of record
                tmpRecordPtr = malloc(sizeof(struct patientRecord));
                strcpy(tmpRecordPtr->recordID, data[i]);
                break;
            case 1:
                strcpy(tmpRecordPtr->patientFirstName, data[i]);
                break;
            case 2:
                strcpy(tmpRecordPtr->patientLastName, data[i]);
                break;
            case 3:
                strcpy(tmpRecordPtr->diseaseID, data[i]);
                break;
            case 4:
                strcpy(tmpRecordPtr->country, data[i]);
                break;
            case 5:
                strcpy(tmpRecordPtr->entryDate, data[i]);
                break;
            case 6:
                strcpy(tmpRecordPtr->exitDate, data[i]);
                break;
        }
    }


    fclose(fp);
}

int main (int argc, char *argv[]) {

    if (!validArgs(argc, argv)) {
        printf("The arguments given are invalid. Try again.\n");
        return -1;
    }

    int diseaseHashTableNumOfEntries, countryHashTableNumOfEntries, bucketSize;
    char *patientRecordsFile;
    getArgs(&diseaseHashTableNumOfEntries, &countryHashTableNumOfEntries, &bucketSize, &patientRecordsFile, argv);

    storeData(patientRecordsFile);

    return 0;
}