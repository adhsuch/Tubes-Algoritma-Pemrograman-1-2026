#include "webgraph.h"
#include "openpage.h"
#include "page_manager.h"

int isValidNumeric(const char *str) {
    int len = strlen(str);
    if (len == 0 || len > 6) return 0;
    for (int i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') return 0;
    }
    int val = atoi(str);
    if (val <= 0 || val > 999999) return 0;
    return 1;
}

/* --- IMPLEMENTASI ADT GRAPH DASAR --- */
void CreateGraph(int x, Graph *g) {
    FIRST(*g) = newGraphNode(x);
}

AdrNode newGraphNode(int x) {
    AdrNode p = (AdrNode) malloc(sizeof(Node));
    if (p != NIL) {
        ID(p) = x;
        NPRED(p) = 0;
        TRAIL(p) = NIL;
        NEXT(p) = NIL;
    }
    return p;
}

AdrSuccNode newSuccNode(AdrNode pn) {
    AdrSuccNode p = (AdrSuccNode) malloc(sizeof(SuccNode));
    if (p != NIL) {
        SUCC(p) = pn;
        NEXT(p) = NIL;
    }
    return p;
}

AdrNode searchNode(Graph g, int x) {
    AdrNode current = FIRST(g);
    while (current != NIL) {
        if (ID(current) == x) {
            return current;
        }
        current = NEXT(current);
    }
    return NIL;
}

void insertNode(Graph *g, int x, AdrNode *pn) {
    AdrNode exist = searchNode(*g, x);
    if (exist != NIL) {
        *pn = exist;
        return;
    }

    AdrNode newNode = newGraphNode(x);
    if (newNode == NIL) {
        *pn = NIL;
        return;
    }

    *pn = newNode;
    if (FIRST(*g) == NIL || ID(FIRST(*g)) > x) {
        NEXT(newNode) = FIRST(*g);
        FIRST(*g) = newNode;
    } else {
        AdrNode current = FIRST(*g);
        while (NEXT(current) != NIL && ID(NEXT(current)) < x) {
            current = NEXT(current);
        }
        NEXT(newNode) = NEXT(current);
        NEXT(current) = newNode;
    }
}

void insertEdge(Graph *g, int prec, int succ) {
    AdrNode pPrec, pSucc;
    insertNode(g, prec, &pPrec);
    insertNode(g, succ, &pSucc);

    if (pPrec != NIL && pSucc != NIL) {
        AdrSuccNode newSucc = newSuccNode(pSucc);
        if (newSucc != NIL) {
            if (TRAIL(pPrec) == NIL) {
                TRAIL(pPrec) = newSucc;
            } else {
                AdrSuccNode lastSucc = TRAIL(pPrec);
                while (NEXT(lastSucc) != NIL) lastSucc = NEXT(lastSucc);
                NEXT(lastSucc) = newSucc;
            }
            NPRED(pSucc)++;
        }
    }
}

/* --- MANAJEMEN GRAF OLEH PAGE MANAGER --- */

void wg_read_linked_pages(Graph *g, int source_id) {
    char url[MAX_STR];
    while (fgets(url, sizeof(url), stdin) != NIL) {
        url[strcspn(url, "\r\n")] = '\0';
        if (strcmp(url, "DONE") == 0) break;
        if (!isValidUrl(url)) {
            printf("Format URL tidak valid!\n"); continue;
        }
        int target_id = pm_get_page_id(url);
        if (target_id == -1) {
            printf("URL tidak ditemukan!\n"); continue;
        }
        insertEdge(g, source_id, target_id);
    }
}

void wg_clear_out_edges(Graph *g, int source_id) {
    AdrNode p = searchNode(*g, source_id);
    if (p != NIL) {
        AdrSuccNode succ = TRAIL(p);
        while (succ != NIL) {
            AdrSuccNode temp = succ;
            succ = NEXT(succ);
            free(temp);
        }
        TRAIL(p) = NIL;
    }
}

void wg_remove_all_edges_related_to(Graph *g, int page_id) {
    AdrNode p = FIRST(*g);
    while (p != NIL) {
        if (ID(p) == page_id) {
            wg_clear_out_edges(g, page_id);
        } else {
            AdrSuccNode prev = NIL;
            AdrSuccNode curr = TRAIL(p);
            while (curr != NIL) {
                if (ID(SUCC(curr)) == page_id) {
                    AdrSuccNode temp = curr;
                    if (prev == NIL) TRAIL(p) = NEXT(curr);
                    else NEXT(prev) = NEXT(curr);
                    curr = NEXT(curr);
                    free(temp);
                } else {
                    prev = curr;
                    curr = NEXT(curr);
                }
            }
        }
        p = NEXT(p);
    }
}

void initGraph(Graph *g) {
    CreateGraph(1, g);
    AdrNode dummy;
    insertNode(g, 2, &dummy);
    insertNode(g, 3, &dummy);
    insertNode(g, 4, &dummy);
    insertNode(g, 5, &dummy);

    insertEdge(g, 1, 4);
    insertEdge(g, 1, 5);
    insertEdge(g, 2, 1);
    insertEdge(g, 3, 2);
}

/* --- IMPLEMENTASI F09 & S01 --- */

void displayLinked(Graph g, int currentId) {
    AdrNode pNode = searchNode(g, currentId);
    if (pNode == NIL || TRAIL(pNode) == NIL) {
        printf("Linked pages:\n(Tidak ada tautan)\n");
        return;
    }

    printf("Linked pages:\n");
    AdrSuccNode pSucc = TRAIL(pNode);
    int index = 1;
    while (pSucc != NIL) {
        int targetId = ID(SUCC(pSucc));
        WebPage *pages = pm_get_web_pages();
        for (int k = 0; k < pm_get_web_page_count(); k++) {
            if (pages[k].id == targetId && pages[k].active) {
                printf("[%d] %s\n", index, pages[k].url);
                break;
            }
        }
        index++;
        pSucc = NEXT(pSucc);
    }
}

void openLinked(Graph g, int currentId, const char *inputX, char *nextUrlToOpen) {
    nextUrlToOpen[0] = '\0';
    if (currentId == -1) {
        printf("ERROR: Command hanya dapat digunakan saat halaman web terbuka!\n");
        return;
    }
    if (!isValidNumeric(inputX)) {
        printf("Input tidak valid.\n"); return;
    }
    int targetIndex = atoi(inputX);
    AdrNode pNode = searchNode(g, currentId);
    if (pNode == NIL || TRAIL(pNode) == NIL) {
        printf("ERROR: Halaman tidak memiliki tautan yang bisa dibuka!\n");
        return;
    }
    AdrSuccNode pSucc = TRAIL(pNode);
    int current = 1, targetWebId = -1;
    while (pSucc != NIL) {
        if (current == targetIndex) {
            targetWebId = ID(SUCC(pSucc)); break;
        }
        current++; pSucc = NEXT(pSucc);
    }
    if (targetWebId != -1) {
        WebPage *pages = pm_get_web_pages();
        for (int k = 0; k < pm_get_web_page_count(); k++) {
            if (pages[k].id == targetWebId && pages[k].active) {
                strcpy(nextUrlToOpen, pages[k].url);
                return;
            }
        }
    }
    printf("Pilihan tidak ada dalam daftar tautan.\n");
}
