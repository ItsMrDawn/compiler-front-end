#ifndef DEFS_H
#define DEFS_H

#include "tk.h"

struct pal_res {
	char palavra[20];
	int tk;
};

struct ops {
	char op[3];
	int tk;
};

extern struct pal_res lista_pal[];

extern const char* str_tokens[];

extern struct ops lista_ops[];

#endif