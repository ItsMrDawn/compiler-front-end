#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabsimb.h"

const char* str_tipos[] = {
	"Int",
    "Float",
    "Char",
    "Double",
    "Long Int",
    "Long Long Int",

    // vetores
    "Vetor Int",
    "Vetor Float",
    "Vetor Char",
    "Vetor Double",
    "Vetor Long Int",
    "Vetor Long Long Int"
};

struct Simbolo tabsimb[100];
int numsimb = 0;

int insere_simbolo_(char nome[50], int tipo) {

    // teste se ja existe
    for (int i = 0; i < numsimb; i++) {
        if (strcmp(tabsimb[i].nome, nome) == 0) {
            return 0;
        }
    }
    
    // insere
    strcpy(tabsimb[numsimb].nome, nome);
    tabsimb[numsimb].tipo = tipo;
    numsimb++;
    
    return 1;
}

int insere_simbolo(Simbolo simb) {
    return insere_simbolo_(simb.nome, simb.tipo);
}

int getsimbolo(char nome[50], int* tipo) {

    for (int i = 0; i < numsimb; i++) {
        if (strcmp(tabsimb[i].nome, nome) == 0) {
            *tipo = tabsimb[i].tipo;
            return 1; // Símbolo encontrado
        }
    }
    
    return 0; // Símbolo não encontrado
}