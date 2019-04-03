#ifndef _LIST_H
#define _LIST_H

#include <stdint.h>
#include <size_t.h>

#define LIST_BEFORE -1
#define LIST_EQUAL 0
#define LIST_AFTER 1

// Data structure used for the look up table in vga
typedef struct List
{
    uint16_t y;
    uint16_t x;

    struct List* prev;
    struct List* next;
} List;

// Create a list item in memory
List* List_CreateItem(uint16_t y, uint16_t x);

// Insert a value into the list (makes the entry)
List* List_Insert(List* list, uint16_t y, uint16_t x);

// Attatches the newList to the source list
List* List_AttatchToList(List* source, List* newList);

// Gets the first element in the list
List* List_GetFirst(List* source);

// Adds an element between two elements in the list
void List_AddBetween(List* element, List* before, List* after);

// Compares one list item with another
// -1 indicated that the first is before the second
// 0 indicates they are the same
// 1 indicates that the first is after the second
int List_Compare(List* lhs, List* rhs);

#endif