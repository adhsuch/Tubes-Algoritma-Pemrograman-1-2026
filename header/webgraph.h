/* === S01 & F09 - Web Graph === */

#ifndef WEBGRAPH_H
#define WEBGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "webpage.h"

#define NIL NULL

typedef struct node *AdrNode;
typedef struct succNode *AdrSuccNode;

typedef struct node {
    int id;
    int nPred;
    AdrSuccNode trail;
    AdrNode next;
} Node;

typedef struct succNode {
    AdrNode succ;
    AdrSuccNode next;
} SuccNode;

typedef struct {
    AdrNode first;
} Graph;

#define FIRST(g) (g).first
#define ID(p) (p)->id
#define NPRED(p) (p)->nPred
#define TRAIL(p) (p)->trail
#define NEXT(p) (p)->next
#define SUCC(p) (p)->succ

/* --- PROTOTYPE VALIDASI E02 --- */

int isValidNumeric(const char *str);
/* I.S. str terdefinisi */
/* F.S. Mengembalikan 1 jika str adalah bilangan bulat positif <= 999999 (E02), 0 jika tidak */

/* --- PROTOTYPE ADT GRAPH DASAR --- */

void CreateGraph(int x, Graph *g);
/* I.S. Sembarang */
/* F.S. Terbentuk graph g dengan satu simpul ber-ID x */
/*      FIRST(*g) menunjuk ke simpul tersebut */

AdrNode newGraphNode(int x);
/* Mengembalikan address hasil alokasi simpul x */
/* Jika alokasi berhasil, maka ID(p)=x, NPRED(p)=0, TRAIL(p)=NIL, NEXT(p)=NIL */
/* Jika alokasi gagal, mengembalikan NIL */

AdrSuccNode newSuccNode(AdrNode pn);
/* Mengembalikan address hasil alokasi successor node */
/* Jika alokasi berhasil, maka SUCC(p)=pn dan NEXT(p)=NIL */
/* Jika alokasi gagal, mengembalikan NIL */

AdrNode searchNode(Graph g, int x);
/* Mengembalikan address simpul dengan ID x jika ada pada graph g */
/* Mengembalikan NIL jika tidak ditemukan */

void insertNode(Graph *g, int x, AdrNode *pn);
/* I.S. g terdefinisi */
/* F.S. Jika alokasi berhasil, simpul x ditambahkan pada leader list */
/*      secara terurut membesar berdasarkan ID, dan pn berisi address simpul x */
/*      Jika x sudah ada, g tidak berubah dan pn berisi address simpul tersebut */
/*      Jika alokasi gagal, g tetap dan pn berisi NIL */

void insertEdge(Graph *g, int prec, int succ);
/* I.S. g, prec, dan succ terdefinisi */
/* F.S. Jika belum ada busur <prec, succ>, busur tersebut ditambahkan ke g */
/*      Jika simpul prec/succ belum ada, simpul tersebut ditambahkan terlebih dahulu */
/*      Jika busur sudah ada, g tidak berubah */

/* --- MANAJEMEN GRAF OLEH PAGE MANAGER --- */

void wg_read_linked_pages(Graph *g, int source_id);

void wg_clear_out_edges(Graph *g, int source_id);

void wg_remove_all_edges_related_to(Graph *g, int page_id);

void initGraph(Graph *g);

/* --- PROTOTYPE F09 & S01 --- */

void displayLinked(Graph g, int currentId);
/* Menampilkan tautan halaman dari node saat ini */

void openLinked(Graph g, int currentId, const char *inputX, char *nextUrlToOpen);
/* F09: Memvalidasi input X (E02), mencari URL target, dan memvalidasi URL target (E01) */

#endif
