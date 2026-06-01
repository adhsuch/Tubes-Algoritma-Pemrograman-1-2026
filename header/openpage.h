/* === F03 - Open Page === */

#ifndef OPENPAGE_H
#define OPENPAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "cache.h"
#include "webgraph.h"
#include "tabs.h"

#define MAX_STR 255

/* --- PROTOTYPE VALIDASI E01 --- */

int isValidUrl(const char *url);
/* I.S. url terdefinisi sebagai string */
/* F.S. Mengembalikan 1 jika url memenuhi syarat regex (E01), 0 jika tidak */

/* --- PROTOTYPE F03 --- */

void op_handle_open(const char *url, Cache *cache, Graph *g, Browser *b);

#endif
