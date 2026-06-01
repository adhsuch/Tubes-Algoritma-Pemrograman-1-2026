#include "tabs_history.h"

void initTab(HistoryTab *tab) {
    tab->currentIndex = -1;
    tab->totalHistory = 0;
}

void visitPage(HistoryTab *tab, char url[]) {
    int i;

    if (tab->totalHistory >= MAX_HISTORY) {
        printf("History penuh.\n");
        return;
    }

    if (tab->currentIndex < tab->totalHistory - 1) {
        tab->totalHistory = tab->currentIndex + 1;
    }

    for (i = 0; url[i] != '\0'; i++) {
        tab->history[tab->totalHistory][i] = url[i];
    }

    tab->history[tab->totalHistory][i] = '\0';

    tab->totalHistory++;
    tab->currentIndex++;

    printf("OPEN: %s\n", url);
}

void viewTabHistory(HistoryTab *tab) {
    int i;

    if (tab->totalHistory == 0) {
        printf("History kosong.\n");
        return;
    }

    printf("\n=== TAB HISTORY ===\n");

    for (i = 0; i < tab->totalHistory; i++) {
        printf("[%d] %s", i, tab->history[i]);

        if (i == tab->currentIndex) {
            printf(" <- YOU ARE HERE");
        }

        printf("\n");
    }

    printf("\n");
}

void backPage(HistoryTab *tab, int step) {
    int oldIndex;

    if (tab->totalHistory == 0) {
        printf("History kosong.\n");
        return;
    }

    if (step <= 0) {
        printf("Step BACK harus lebih dari 0.\n");
        return;
    }

    oldIndex = tab->currentIndex;

    tab->currentIndex = tab->currentIndex - step;

    if (tab->currentIndex < 0) {
        tab->currentIndex = 0;
    }

    printf(
        "BACK: %s -> %s # INDEX %d -> %d\n",
        tab->history[oldIndex],
        tab->history[tab->currentIndex],
        oldIndex,
        tab->currentIndex
    );
}

void forwardPage(HistoryTab *tab, int step) {
    int oldIndex;

    if (tab->totalHistory == 0) {
        printf("History kosong.\n");
        return;
    }

    if (step <= 0) {
        printf("Step FORWARD harus lebih dari 0.\n");
        return;
    }

    oldIndex = tab->currentIndex;

    tab->currentIndex = tab->currentIndex + step;

    if (tab->currentIndex >= tab->totalHistory) {
        tab->currentIndex = tab->totalHistory - 1;
    }

    printf(
        "FORWARD: %s -> %s # INDEX %d -> %d\n",
        tab->history[oldIndex],
        tab->history[tab->currentIndex],
        oldIndex,
        tab->currentIndex
    );
}
