#ifndef DISCOVER_H
#define DISCOVER_H
 
#include <stdio.h>
#include "webpage.h"

#define DISCOVER_COUNT 5
 
/* LCG */
 
/* Prosedur: Set seed awal LCG */
void discover_seed(unsigned int seed);
 
/* Fungsi: Hasilkan bilangan acak berikutnya menggunakan LCG
Rumus: X(n+1) = (a * X(n) + c) mod m */
unsigned int discover_lcg_next(void);
 
/* Prosedur: Jalankan fitur Discover */
void discover_run(WebPage web_pages[], int web_page_count);
 
#endif 