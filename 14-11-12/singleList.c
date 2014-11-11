/*
    Singly linked list
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include <stdlib.h>
#include "singleList.h"

struct Item {
    int val;
    struct Item *next;
} *head = NULL;

// adds {newValue} to the top of the list
void list_add(int newValue) {
    struct Item *temp = (struct Item *)malloc(sizeof(struct Item));
    if (temp == NULL) {
        printf("Error: out of memory!\n");
        return;
    }
    temp->val = newValue;
    temp->next = head;
    head = temp;
}

// initializes an empty list
void list_init(void) {
    while (head != NULL) {
        list_pop();
    }
}

// removes the first occurence of {value} in the list
void list_removeByValue(int value) {
    if (head == NULL) {
        return;
    }
    struct Item *cur = head->next, *prev = head;
    while (cur != NULL) {
        if (cur->val == value) {
            prev->next = cur->next;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

// removes the first item of the list
void list_pop(void) {
    if (head != NULL) {
        struct Item *rest = head->next;
        free(head);
        head = rest;
    }
}

// prints all items of the list (to stdio, divided by space)
void list_print(void) {
    struct Item *cur = head;
    while (cur != NULL) {
        printf("%d ", cur->val);
        cur = cur->next;
    }
    printf("\n");
}
