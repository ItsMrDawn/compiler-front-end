#ifndef TOKEN_H
#define TOKEN_H

#define TAM_MAX_LEX 50

// Estrutura para armazenar as informações de um token
typedef struct Token {
    int lin;
    int col;
    int num; // um numero sequencial que representa a ordem
    int tk;
    char lexema[TAM_MAX_LEX];
} Token;

#endif