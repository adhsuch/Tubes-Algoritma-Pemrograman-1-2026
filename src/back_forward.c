#include "back_forward.h"

WebPage webPageEmpty(void) {
    WebPage page;

    page.id = 0;
    page.url[0] = '\0';
    page.content[0] = '\0';
    page.active = 0;

    return page;
}

void stackInit(PageStack *stack) {
    stack->top = NULL;
    stack->size = 0;
}

int stackIsEmpty(const PageStack *stack) {
    return stack->top == NULL;
}

int stackPush(PageStack *stack, WebPage page) {
    StackNode *node;

    if (stack->size >= MAX_HISTORY_SIZE) return 0;

    node = (StackNode *)malloc(sizeof(StackNode));
    if (node == NULL) return 0;

    node->page = page;
    node->next = stack->top;
    stack->top = node;
    stack->size++;

    return 1;
}

WebPage stackPop(PageStack *stack) {
    StackNode *node;
    WebPage page;

    if (stackIsEmpty(stack)) return webPageEmpty();

    node = stack->top;
    page = node->page;
    stack->top = node->next;
    free(node);
    stack->size--;

    return page;
}

void stackDestroy(PageStack *stack) {
    while (!stackIsEmpty(stack)) {
        stackPop(stack);
    }
}

void navInit(NavHistory *nav) {
    stackInit(&nav->back);
    stackInit(&nav->forward);
    nav->current  = webPageEmpty();
    nav->has_page = 0;
}

void navDestroy(NavHistory *nav) {
    stackDestroy(&nav->back);
    stackDestroy(&nav->forward);
    nav->has_page = 0;
}

void navDisplayCurrentPage(const NavHistory *nav) {
    if (!nav->has_page) { printf("(Tab kosong)\n"); return; }
    printf("%s\n", nav->current.content);
}

void navOpenPage(NavHistory *nav, WebPage page) {
    if (nav->has_page) {
        stackDestroy(&nav->forward);
        stackInit(&nav->forward);
        stackPush(&nav->back, nav->current);
    }
    nav->current  = page;
    nav->has_page = 1;
}

void navBack(NavHistory *nav) {
    if (!nav->has_page) {
        printf("ERROR: BACK TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n");
        return;
    }
    if (stackIsEmpty(&nav->back)) {
        printf("ERROR: BACK TIDAK BISA DIGUNAKAN LAGI KARENA TIDAK ADA HALAMAN SEBELUMNYA!\n\n");
        navDisplayCurrentPage(nav);
        return;
    }
    stackPush(&nav->forward, nav->current);
    nav->current = stackPop(&nav->back);
    printf("BACK: KEMBALI KE HALAMAN %s\n\n", nav->current.url);
    navDisplayCurrentPage(nav);
}

void navForward(NavHistory *nav) {
    if (!nav->has_page || stackIsEmpty(&nav->forward)) {
        printf("ERROR: FORWARD TIDAK BISA DIJALANKAN KARENA TIDAK ADA HALAMAN SELANJUTNYA!\n");
        if (nav->has_page) navDisplayCurrentPage(nav);
        return;
    }
    stackPush(&nav->back, nav->current);
    nav->current = stackPop(&nav->forward);
    printf("FORWARD: KEMBALI KE HALAMAN %s\n\n", nav->current.url);
    navDisplayCurrentPage(nav);
}
