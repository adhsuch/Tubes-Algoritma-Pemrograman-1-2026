#ifndef TABS_HISTORY_H
#define TABS_HISTORY_H

#include <stdio.h>
#include <string.h>

#define MAX_HISTORY 100
#define MAX_URL 100

typedef struct {
    char history[MAX_HISTORY][MAX_URL];
    int currentIndex;
    int totalHistory;
} HistoryTab;

void initTab(HistoryTab *tab);

void visitPage(HistoryTab *tab, char url[]);

void viewTabHistory(HistoryTab *tab);

void backPage(HistoryTab *tab, int step);

void forwardPage(HistoryTab *tab, int step);

#endif
