#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Interface.h"

void printUniqueList(uniqueNamesList *head) {

    uniqueNamesList *current = head;
    while (current != NULL) {
        printf("%s | %d\n", current->nameId, current->counter);
        current = current->next;
    }
}

void insertUnique(uniqueNamesList **head, char *name) {

    printf("mpike\n");
    uniqueNamesList *newNode = (uniqueNamesList*) malloc(sizeof(uniqueNamesList)); // allocating the new node
    newNode->next = NULL;
    newNode->nameId = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(newNode->nameId, name);
    newNode->counter = 0;
    printf("alloc mesa ok\n");

    uniqueNamesList *tmp = *head;

    if(*head == NULL) { // if list is empty
        *head = newNode;
        printf("ok 1st\n");
        return;
    }

    while(tmp->next != NULL) { // search the list by going to its tail
        if(strcmp(newNode->nameId, tmp->nameId) == 0) {
            // if there is already this name on the list, free the new node and exit
            free(newNode->nameId);
            free(newNode);
            return;
        }

        tmp = tmp->next;
    }
    // check last record too
    if(strcmp(newNode->nameId, tmp->nameId) == 0) {
        // if there is already this name on the list, free the new node and exit
        free(newNode->nameId);
        free(newNode);
        return;
    }

    // store the node at the end
    tmp->next = newNode;
}
