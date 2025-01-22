#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lex.h"

// handles viraram globais agora
FILE *arq_in, *arq_out;

int palavra_reservada(char lex[]) {

	int postab = 0;

	while (strcmp("fimtabela", lista_pal[postab].palavra) != 0) {

		if (strcmp(lex, lista_pal[postab].palavra) == 0) {
			return lista_pal[postab].tk;
		}
		postab++;
	}

	return TKId;
}

// se o char é um operador de 1 ou 2 caracteres
int is_operador(char c1, char* c2) {

	int i = 0;

	//testar os dois caracteres

	while (lista_ops[i].op[0] != '$') {

		if ((lista_ops[i].op[0] == c1) && (lista_ops[i].op[1] == *c2)) {
			return lista_ops[i].tk;
		}
		i++;
	}

	// se não reconheceu, deve ser um com 1 caracter
	// zerar o segundo pra sinalizar, e segue a lista

	*c2 = 0;
	i++;

	while (lista_ops[i].op[0] != '$') {

		if (lista_ops[i].op[0] == c1) {
			//printf("Meu retorno é %s\n", str_tokens[lista_ops[i].tk]);
			return lista_ops[i].tk;
		}
		i++;
	}

	// nenhum encontrado

	return TKErro;

}

// printar o token no formato
void print_token(Token token) {

	//printf("%-4d\t%-4d\t%-20s\t%-20s\n", token.lin, token.col, str_tokens[token.tk], token.lexema);
	fprintf(arq_out, "%-4d\t%-4d\t%-20s\t%-20s\n", token.lin, token.col, str_tokens[token.tk], token.lexema);

}

// alfanumerico
int ler_alfa(char c, char* lexema, int* col) {

	int lex_index = 0;

	// enquanto o char for alfanumerico, consumir o identificador
	while (isalnum(c) || c == '_') {
		lexema[lex_index++] = c;
		(*col)++;
		fread(&c, 1, 1, arq_in);
	}

	lexema[lex_index] = '\0'; // terminar string
	fseek(arq_in, -1, SEEK_CUR); // voltar 1 espaço no arq de entrada

	return palavra_reservada(lexema);

}

int ler_numero(char c, char* lexema, int* col) {

	int tk;
	int lex_index = 0;

	// assumir que é um integer
	tk = TKIntegerLiteral;

	while (isdigit(c) || c == '.') {

		lexema[lex_index++] = c;
		(*col)++;

		// se tem um . trata-se de um float, continuar lendo
		if (c == '.') {
			tk = TKFloatLiteral;
		}

		fread(&c, 1, 1, arq_in);
	}

	lexema[lex_index] = '\0';    // terminar string
	fseek(arq_in, -1, SEEK_CUR); // voltar 1 espaço no arq de entrada

	return tk;

}

// identificar operador
int ler_operador(char c, char* lexema, int* col) {

	// ler o proximo
	char c2;
	fread(&c2, 1, 1, arq_in);

	// ver se encaixa em um dos operadores
	int tk = is_operador(c, &c2);

	int lex_index = 0;
	(*col)++;

	// não é um operador válido
	if (tk == TKErro) {
		return tk;
	}

	lexema[lex_index++] = c;

	// se o segundo caracter for lido
	if (c2 != 0) {
		lexema[lex_index++] = c2;
		(*col)++;
	} else {
		// voltar 1
		fseek(arq_in, -1, SEEK_CUR);
	}

	lexema[lex_index] = '\0';

	// se era o inicio de um comentario ou string, ler até o final
	if ((tk == TKComentario) || (tk == TKString)) {

		lex_index--;
		char fim = '"';

		if (tk == TKComentario) {
			fim = '\n';
		}

		// só esperar o final
		do {
			lexema[lex_index++] = c;
			(*col)++;
		} while ((c = fgetc(arq_in)) != fim);

		// ultimo "
		if (tk == TKString) {
			lexema[lex_index++] = c;
		}

		lexema[lex_index] = '\0';
	}

	return tk;

}

// função principal
Token ler_token_lex() {

	static int pos = 1;  // posição atual da coluna, quando começou a ler o token
	static int cont = 1; // um numero sequencial que representa a ordem
	static Token token = {.lin = 1};

	char c;

	token.col = pos; // receber a coluna que foi incrementada antes (pos == depois do ultimo token)

	while (1) { // laço para pular espaços e newlines

		fread(&c, 1, 1, arq_in);

		// fim do arquivo
		if (feof(arq_in)) {
			token.tk = TKEOF;
			token.lexema[0] = '\0';
			return token;
		}

		// newline, incrementar linha
		if (c == '\n') {
			token.lin++;
			token.col = 1;
			continue;
		} else if (isspace(c)) { // espaço ou tab, apenas incrementar coluna
			token.col++;
			continue;
		}

		break;

	}

	pos = token.col;

	if (isalpha(c) || c == '_') { // caracter alfabetico

		token.tk = ler_alfa(c, token.lexema, &pos); // se não encontrar nada, assume que é identificador

	} else if (isdigit(c)) { // numero

		token.tk = ler_numero(c, token.lexema, &pos);

	} else  { // operador de 1 ou 2 caracteres

		token.tk = ler_operador(c, token.lexema, &pos); // se não encontrar nada, assume que é erro

	}

	token.num = cont++;

	print_token(token);

	return token;

}


int init_lex(char nomearq[]) {

	arq_in  = fopen(nomearq, "rb"); //rb para windows
	arq_out = fopen("out_tokens.txt", "w");

	if (arq_in == NULL || arq_out == NULL) {
		printf("Erro ao abrir os arquivos.\n");
		return 1;
	}

	//printf("Linha   Col     Token                   Lexema\n");
	fprintf(arq_out, "Linha   Col     Token                   Lexema\n");

	return 0;
}

int fechar_lex() {

	fclose(arq_in);
	fclose(arq_out);

}
