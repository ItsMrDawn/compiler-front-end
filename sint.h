#ifndef SINT_H
#define SINT_H

void get_token(void);
void goto_pos(int pos);
void desempilhar(int pos);
void print_erro(char esperado[]);
int programa(void);
int resto_programa(void);
int declaracoes(void);
int declaracao(void);
int lista_identificadores(void);
int tipo(void);
int modificadores(void);
int resto_modificadores(void);
int modificador(void);
int funcao(void);
int lista_parametros(void);
int resto_lista_parametros(void);
int parametro(void);
int bloco_chave(void);
int lista_comandos(void);
int resto_lista_comandos(void);
int comando(void);
int comando_controle(void);
int while_loop(void);
int do_while_loop(void);
int for_loop(void);
int comando_if(void);
int comando_else(void);
int expressao(void);
int resto_expressao(void);
int expressao_atrib(void);
int expressao_logica(void);
int resto_expressao_logica(void);
int expressao_igualdade(void);
int resto_expressao_igualdade(void);
int expressao_relacional(void);
int resto_expressao_relacional(void);
int expressao_soma(void);
int resto_expressao_soma(void);
int expressao_mult(void);
int resto_expressao_mult(void);
int expressao_unaria(void);
int expressao_primaria(void);
int constante(void);
int char_literal(void);
int chamada_funcao(void);
int argumentos(void);
int resto_argumentos(void);
int operador_atrib(void);
int operador_logico(void);
int operador_igualdade(void);
int operador_relacional(void);
int operador_soma(void);
int operador_mult(void);
int operador_unario(void);
int controle_laco(void);
int retorno(void);
int main(int argc, char **argv);

#endif