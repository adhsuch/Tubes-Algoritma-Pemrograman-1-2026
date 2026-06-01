#ifndef BACK_FORWARD_H
#define BACK_FORWARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"

#define MAX_HISTORY_SIZE 100

typedef struct StackNode {
    WebPage page;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    int size;
} PageStack;

typedef struct {
    PageStack back;
    PageStack forward;
    WebPage current;
    int has_page;
} NavHistory;

WebPage webPageEmpty(void);

void stackInit(PageStack *stack);
int stackIsEmpty(const PageStack *stack);
int stackPush(PageStack *stack, WebPage page);
WebPage stackPop(PageStack *stack);
void stackDestroy(PageStack *stack);

void navInit(NavHistory *nav);
void navDestroy(NavHistory *nav);
void navDisplayCurrentPage(const NavHistory *nav);
void navOpenPage(NavHistory *nav, WebPage page);
void navBack(NavHistory *nav);
void navForward(NavHistory *nav);

#endif
