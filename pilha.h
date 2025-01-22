#ifndef PILHA_H
#define PILHA_H

#include <stdlib.h>

// a struct do token que vai ser passado pro sintatico
#include "token.h"

// onde tem o enum dos tokens
#include "tk.h"

// um nó da pilha
typedef struct Nodo {
    struct Nodo* ant;
    struct Nodo* prox;
    Token token;
} Nodo;

void push(Token token);

Token pop();

// desempilhar até o token tk
Token pop_until(int tk);

// retornar o valor do topo
Token get_topo();

// se está no topo da pilha
int is_topo();

// proximo token da pilha
Token avancar();

// token anterior
Token voltar();

// voltar até a posição de num na pilha
Token voltar_until(int num);

#endif