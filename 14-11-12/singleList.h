/*
    Singly linked list
    Author: Kirill Smirenko, group 171
*/

#ifndef SINGLELIST_H
#define SINGLELIST_H

// adds {newValue} to the top of the list
void list_add(int newValue);

// initializes an empty list
void list_init(void);

// removes the first occurence of {value} in the list
void list_removeByValue(int value);

// removes the first item of the list
void list_pop(void);

// prints all items of the list (to stdio, divided by space)
void list_print(void);

#endif
