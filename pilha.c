#include <stdio.h>
#include <stdlib.h>

#include "pilha.h"

Nodo* pilha = NULL;

// Função para realizar o push na pilha
void push(Token token) {

    Nodo* novo = (Nodo*)malloc(sizeof(Nodo));

    novo->token = token;
    novo->ant   = NULL;
    novo->prox  = NULL;

    if (pilha != NULL) {
        novo->ant   = pilha;
        pilha->prox = novo;
    }
    
    // ponteiro da pilha agora aponta pro novo
    pilha = novo;

}

Token pop() {

    Token token;

    if (pilha == NULL) {
        printf("Erro - pilha vazia.\n");
        token.tk = TKErro;
        return token;
    }

    Nodo* topo = pilha;

    pilha = topo->ant;

    if (pilha != NULL) {
        pilha->prox = NULL;
    }

    token = topo->token;

    free(topo);

    return token;

}

// desempilhar até a posicao pos
Token pop_until(int pos) {

    Token token;  

    while (pilha != NULL) {

        token = pop();

        if (token.num == pos) {
            break;
        }

    }

    return get_topo();

}

// retornar o valor do topo
Token get_topo() {

    if (pilha == NULL) {
        printf("Erro - pilha vazia.\n");
        Token token = {.tk = TKErro};
        return token;
    }

    return pilha->token;
}

// retornar o valor do proximo token, se houver
Token peek_prox() {

    if (pilha == NULL) {
        printf("Erro - pilha vazia.\n");
        Token token = {.tk = TKErro};
        return token;
    }

    if (pilha->prox == NULL) {
        return pilha->token;
    } else {
        return pilha->prox->token;
    }

    
}

int is_topo() {
    return ((pilha == NULL) || (pilha->prox == NULL));
}

// avançar 1 na pilha
Token avancar() {

    if (!is_topo()) {
        pilha = pilha->prox;
    }

    return get_topo();

}

Token voltar() {

    if (pilha->ant != NULL) {
        pilha = pilha->ant;
    }

    return get_topo();

}

// voltar até a posição de num na pilha
Token voltar_until(int num) {

    while (num != pilha->token.num) {

        voltar();

    }

    return get_topo();

}

