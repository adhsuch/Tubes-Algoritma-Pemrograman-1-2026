#include "tabs.h"

void InitBrowser(Browser *b) {

    b->count = 1;
    b->current = 0;
    b->nextId = 2;

    strcpy(b->tabs[0].name, "TAB1");
}

void CheckTab(Browser *b) {

    int i;

    printf("List of tab(s):\n");

    for (i = 0; i < b->count; i++) {

        printf("[%d] %s\n", i + 1, b->tabs[i].name);
    }

    printf("Current tab: %s\n", b->tabs[b->current].name);
}

void NewTab(Browser *b) {

    if (b->count >= TABS_MAX_AMOUNT) {

        printf("ERROR: Jumlah tab tidak bisa melebihi batas maksimum!\n");
        return;
    }

    sprintf(b->tabs[b->count].name, "TAB%d", b->nextId);

    printf("Tab baru (%s) berhasil dibuat!\n",
           b->tabs[b->count].name);

    b->count++;
    b->nextId++;
}

void CloseTab(Browser *b) {

    int i;

    if (b->count == 1) {

        printf("ERROR: Tidak bisa menutup tab, tab minimal berjumlah 1!\n");
        return;
    }

    printf("%s berhasil ditutup.\n",
           b->tabs[b->current].name);

    for (i = b->current; i < b->count - 1; i++) {

        b->tabs[i] = b->tabs[i + 1];
    }

    b->count--;

    if (b->current >= b->count) {

        b->current = b->count - 1;
    }
}

void NextTab(Browser *b, int step) {

    int target;

    target = b->current + step;

    if (target >= b->count) {

        printf("ERROR: Posisi tab tidak valid!\n");
        return;
    }

    b->current = target;

    printf("Tab saat ini berhasil diganti ke %s.\n",
           b->tabs[b->current].name);
}

void PrevTab(Browser *b, int step) {

    int target;

    target = b->current - step;

    if (target < 0) {

        printf("ERROR: Posisi tab tidak valid!\n");
        return;
    }

    b->current = target;

    printf("Tab saat ini berhasil diganti ke %s.\n",
           b->tabs[b->current].name);
}
