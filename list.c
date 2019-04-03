#include <list.h>
#include <_null.h>
#include "physicalmemorymanager.h"

//static size_t ListCount = 0;

List* List_CreateItem(uint16_t y, uint16_t x)
{
    List* item;

    item = PMM_AllocateBlock(sizeof(List));

    //ListCount++;
    //item = (((List*)0x0C0000) + ListCount);

    item->y = y;
    item->x = x;
    item->prev = NULL;
    item->next = NULL;

    return item;
}

List* List_Insert(List* list, uint16_t y, uint16_t x)
{
    return List_AttatchToList(list, List_CreateItem(y,x));
}

List* List_AttatchToList(List* source, List* newList)
{
    // This is just for the first case where we wont have a source
    if(!source)
    {
        return newList;
    }

    int comparison = List_Compare(newList, source);

    if(comparison == LIST_BEFORE)
    {
        if(source->prev)
        {
            // If we already have an element before this
            // check if we go between it, else we need to move back to that position

            switch(List_Compare(newList, source->prev))
            {
                case LIST_BEFORE:
                case LIST_EQUAL:
                    // It's before or equal. So we just want to recursively call this
                    List_AttatchToList(source->prev, newList);
                    break;
                case LIST_AFTER:
                    // It's after so we want to put it in between source and source->prev
                    List_AddBetween(newList, source->prev, source);
                    break;
            }
        }
        else
        {
            // Just add it before
            source->prev = newList;
            newList->next = source;
        }
    }
    else if (comparison == LIST_EQUAL)
    {
        // This element and the one being compared is the same, so it doesnt matter if it goes before or after
        newList->next = source->next;
        newList->next->prev = newList;

        source->next = newList;
        newList->prev = source;
    }
    else if (comparison == LIST_AFTER)
    {
        if(source->next)
        {
            // If we already have an element after this
            // Check if we go between it, else we need to go forward to that position

            switch(List_Compare(newList, source->next))
            {
                case LIST_AFTER:
                case LIST_EQUAL:
                    // It's after or equal. So we just want to recursively call this
                    List_AttatchToList(source->next, newList);
                    break;
                case LIST_BEFORE:
                    // It's before so we want to put it in between source and source->next
                    List_AddBetween(newList, source, source->next);
                    break;
            }
        }
        else
        {
            // Just add it after
            source->next = newList;
            newList->prev = source;
        }
    }
    return source;
}

List* List_GetFirst(List* source)
{
    while(source->prev)
    {
        source = source->prev;
    }
    return source;
}

void List_AddBetween(List* element, List* before, List* after)
{
    before->next = element;
    after->prev = element;

    element->next = after;
    element->prev = before;
}

int List_Compare(List* lhs, List* rhs)
{
    if(lhs->y < rhs->y)
    {
        return LIST_BEFORE;
    }
    else if(lhs->y == rhs->y)
    {
        if(lhs->x < rhs->x)
        {
            return LIST_BEFORE;
        }
        else if (lhs->x == rhs->x)
        {
            return LIST_EQUAL;
        }
        else
        {
            return LIST_AFTER;
        }
    }
    else
    {
        return LIST_AFTER;
    }
}