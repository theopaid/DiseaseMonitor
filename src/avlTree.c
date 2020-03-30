#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/Interface.h"

bstNode *newNode(Date keydateValue, listNode *record) {

    bstNode *node = (bstNode *)malloc(sizeof(bstNode));
    node->record = record;
    node->dateValue = keydateValue;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->count = 1;
    printf("INSERTING NODE: KEYdate = %d | ID = %s\n", keydateValue.day, record->record->recordID);
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

    printf("STARTING INSERT FOR: %d | %s\n",keydateValue.year , record->record->recordID);

    if (node == NULL) {
        printf("NULL:OK\n");
        return (newNode(keydateValue, record));
    }
    printf("INSERT:OK1\n");
    Date *nodesDate = &(node->dateValue);
    printf("INSERT:OK2\n");
    printf("insertdates: %d-%d-%d\n", nodesDate->day, nodesDate->month, nodesDate->year);
    if (nodesDate->year==keydateValue.year && nodesDate->month==keydateValue.month && nodesDate->day==keydateValue.day) {
        (node->count)++;
        printf("INSERT:OK3\n");
        return node;
    }

    if (compareStructDates(keydateValue, *nodesDate) == -1) {
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

    if(root != NULL) {
        printf("%d(%d) ", root->dateValue.day, root->count);
        preOrder(root->left);
        preOrder(root->right);
    }
}

int preOrderCounter(bstNode *root) {

    int counter = 0;
    if(root != NULL) {
        printf("%d-%d-%d and %d\n", root->dateValue.day, root->dateValue.month, root->dateValue.year, root->count);
        printf("id: %s\n", root->record->record->recordID);
        counter = root->count;
        counter += preOrderCounter(root->left);
        counter += preOrderCounter(root->right);
    }

    return counter;
}

int preOrderCounterWDates(bstNode *root, Date entryDate, Date exitDate) {

    int counter = 0;
    if(root != NULL) {
        if(compareStructDates(entryDate, root->dateValue) != 1 && compareStructDates(exitDate, root->dateValue) != -1) {
            // enter if entryDate <= treeNode date >= exitDate
            counter = root->count;
        }
        counter += preOrderCounterWDates(root->left, entryDate, exitDate);
        counter += preOrderCounterWDates(root->right, entryDate, exitDate);
    }

    return counter;
}

int preOrderDiseaseCountryCounter(bstNode *root, char *country, Date entryDate, Date exitDate) {

    listNode *currentNode;
    int counter = 0;
    if(root != NULL) {
        if(compareStructDates(entryDate, root->dateValue) != 1 && compareStructDates(exitDate, root->dateValue) != -1) {
            // enter if entryDate <= treeNode date >= exitDate
            if(country != NULL) {
                currentNode = root->record;
                while(currentNode != NULL && compareStructDates(root->dateValue, currentNode->record->entryDate) == 0) {
                    // As long as records have the same entryDate as root
                    if(strcmp(currentNode->record->country, country) == 0) { // if country matches the one in the record
                        counter++;
                    }

                    currentNode = currentNode->next;
                }
            }
            else {
                counter = root->count;
            }
        }
        counter += preOrderDiseaseCountryCounter(root->left, country, entryDate, exitDate);
        counter += preOrderDiseaseCountryCounter(root->right, country, entryDate, exitDate);
    }

    return counter;
}

int preOrderDiseaseCounterWDates(bstNode *root) {

    listNode *currentNode;
    int counter = 0;
    if(root != NULL) {
        currentNode = root->record;
        while(currentNode != NULL && compareStructDates(root->dateValue, currentNode->record->entryDate) == 0) {
            // As long as records have the same entryDate as root
            if(currentNode->record->exitDate.day == 0) { // if there is no Exit date
                counter++;
            }

            currentNode = currentNode->next;
        }

        counter += preOrderDiseaseCounterWDates(root->left);
        counter += preOrderDiseaseCounterWDates(root->right);
    }

    return counter;
}

void prerOrderPrinterWDates(bstNode *root) {

    //printf("mpike\n");
    listNode *currentNode;
    if(root != NULL) {
        currentNode = root->record;
        while(currentNode != NULL && compareStructDates(root->dateValue, currentNode->record->entryDate) == 0) {
            // As long as records have the same entryDate as root
            if(currentNode->record->exitDate.day == 0) { // if there is no Exit date
                printRecord(*(currentNode->record));
            }

            currentNode = currentNode->next;
        }

        prerOrderPrinterWDates(root->left);
        prerOrderPrinterWDates(root->right);
    }
}
