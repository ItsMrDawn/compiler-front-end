#ifndef LEX_H
#define LEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// a struct do token que vai ser passado pro sintatico
#include "token.h"

// onde tem todas definições de tokens e strings associados
#include "defs.h"

// printar o token no formato
void print_token(Token token);

// função principal
Token ler_token_lex();

int init_lex(char nomearq[]);

int fechar_lex();

#endif