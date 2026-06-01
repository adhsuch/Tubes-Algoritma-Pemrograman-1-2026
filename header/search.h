#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "webpage.h"

#define MAX_CONTENT 256


void ToLowerString(char str[]);
void Search(WebPage web[], int n, char query[]);
void AdvancedSearch(WebPage web[], int n, char query[]);

#endif
