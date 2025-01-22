#ifndef TABSIMB_H
#define TABSIMB_H

enum enum_tipo {
    TInt,
    TFloat,
    TChar,
    TDouble,
    TLongInt,
    TLongLongInt,

    //vetores
    TVetInt,
    TVetFloat,
    TVetChar,
    TVetDouble,
    TVetLongInt,
    TVetLongLongInt
};

typedef struct Simbolo {
    char nome[50];
    int tipo;
    
} Simbolo;

int insere_simbolo_(char nome[50], int tipo);

int insere_simbolo(Simbolo simb);

int getsimbolo(char nome[50], int* tipo);

#endif