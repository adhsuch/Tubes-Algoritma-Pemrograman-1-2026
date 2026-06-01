
#ifndef TABS_H
#define TABS_H

#include <stdio.h>
#include <string.h>

#define TABS_MAX_AMOUNT 10
#define MAX_NAME 20

typedef struct {
    char name[MAX_NAME];
} Tab;

typedef struct {
    Tab tabs[TABS_MAX_AMOUNT];
    int count;
    int current;
    int nextId;
} Browser;

void InitBrowser(Browser *b);

void CheckTab(Browser *b);

void NewTab(Browser *b);

void CloseTab(Browser *b);

void NextTab(Browser *b, int step);

void PrevTab(Browser *b, int step);

#endif
