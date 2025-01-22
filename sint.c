#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "sint.h"

// funções da pilha de tokens
#include "pilha.h"

// o analisador lexico
#include "lex.h"

// tabela de simbolos
#include "tabsimb.h"

// um token global
Token token;

// um simbolo global
Simbolo simb;

// sinalizador de erro
int erro = 0;

#define TIS(x) (token.tk == x)
#define TNOT(x) (token.tk != x)

#define SAVE int pos = token.num;
#define LOAD goto_pos(pos);
#define VOLTA1 goto_pos(-1);
#define POPALL desempilhar(pos);

#define GETNOT(token) {get_token(); if TNOT(token) {return 0;}}
#define GETNOTPRINT(token, esperado) {get_token(); if TNOT(token) {print_erro(esperado); return 0;}}

#define IFPROD(producao) if (producao()) {return 1;};
#define IFNPROD(producao) if (!producao()) {return 0;};

#define IFTNOT(token) if TNOT(token) {return 0;}
#define IFTIS(token)  if TIS(token) {return 1;}


void get_token() {

    // se está no topo da pilha, ler mais um do lexico
    if (is_topo()) {
        token = ler_token_lex();
        push(token);
    } else {
        // não é o topo, pegar o proximo da pilha
        token = avancar();
    }

}

void goto_pos(int pos) {

    if (pos != -1) {
        token = voltar_until(pos);
    } else {
        token = voltar();
    }

}

// tirar da pilha até a posição pos
void desempilhar(int pos) {

    token = pop_until(pos);

}

void print_erro(char esperado[]) {

    if (erro) {
        return;
    }

    printf("Erro sintático na linha %d, coluna %d\n", token.lin, token.col);
    printf("Esperado: %s\n", esperado);
    printf("Encontrado: %s\n\n", token.lexema);

    erro++;

}

void print_status(char producao[]) {

    printf("Linha %d, coluna %d\n", token.lin, token.col);
    printf("Token %s\n", str_tokens[token.tk]);
    printf("Produção: %s\n\n", producao);

}

// <programa> 
//   | <declaracao> <resto_programa> 
//   | <funcao> <resto_programa>
int programa () {

    if (declaracao()) {
        GETNOTPRINT(TKPontoVirgula, ";");
        return resto_programa();
    }

    goto_pos(1);

    if (funcao()) {
        return resto_programa();
    }
    
    return 0;

}


// <resto_programa> 
//   | <declaracao> <resto_programa> 
//   | <funcao> <resto_programa> 
//   | ε
int resto_programa() {

    SAVE;

    if (declaracao()) {
        GETNOTPRINT(TKPontoVirgula, ";");
        return resto_programa();
    }

    LOAD;

    if (funcao()) {
        return resto_programa();
    }

    LOAD;
    
    return 1;


}

// <declaracoes>
//   | <declaracao> ';'
//   | <declaracao> ',' <declaracoes> ';'
int declaracoes() {

    IFNPROD(declaracao);

    IFTIS(TKPontoVirgula);

    // não é ';', precisa ser uma virgula pra continuar as declarações

    VOLTA1;
    GETNOTPRINT(TKVirgula, "virgula");

    // é uma virgula, deve ter outra declaração
    IFNPROD(declaracoes);
    
    return 1;
    
}


// <declaracao> 
//  | <tipo> <lista_identificadores> ';'
int declaracao() {

    get_token();

    IFNPROD(tipo);

    // n vou considerar longint e cia por ora
    switch (token.tk) {

        case TKInt:    simb.tipo = TInt;    break;
        case TKFloat:  simb.tipo = TFloat;  break;
        case TKChar:   simb.tipo = TChar;   break;    
        case TKDouble: simb.tipo = TDouble; break;

        default: simb.tipo = TInt;
    }

    IFNPROD(lista_identificadores);

    GETNOTPRINT(TKPontoVirgula, ";");

    return 1;    

}

// <lista_identificadores> 
//   | <identificador> 
//   | <identificador> <modificadores>
//   | <identificador> ',' <lista_identificadores>
int lista_identificadores() {

    GETNOT(TKId);

    // nome
    strcpy(simb.nome, token.lexema);

    // inserir na tabela de simbolos, usando o "tipo" que foi definido na função "declaração"
    if (!insere_simbolo(simb)) {
        printf("Erro sintático na linha %d, coluna %d\n", token.lin, token.col);
        printf("Variavel %s ja declarada\n", simb.nome);

        erro++;
    }

    IFNPROD(modificadores);

    get_token();

    if (TIS(TKVirgula)) {
        return lista_identificadores();
    }
    
    VOLTA1; 

    return 1;
    
}


// <tipo> 
//   | int 
//   | long int 
//   | long long int 
//   | float 
//   | char 
//   | double
int tipo() {

    if (TIS(TKInt) || TIS(TKFloat) || TIS(TKChar) || TIS(TKDouble)) {
        return 1;
    }

    // se nao for um tipo tradicional precisa ser um long
    if TNOT(TKLong) {
        //print_erro("Tipo");
        return 0;
    }

    // se é um long, ler o proximo
    get_token();

    // long int
    if TIS(TKInt) {
        return 1;
    } else if TNOT(TKLong) {
        print_erro("Long");
        return 0;
    }

    // long long int, ler o ultimo int
    get_token();

    // deve ser um int
    if TNOT(TKInt) {
        print_erro("Int");
        return 0;
    } 
    
    return 1;

}


// <modificadores> 
//   | <modificador> <resto_modificadores> 
//   | ε
int modificadores() {

    SAVE;

    if (modificador()) {
        IFPROD(resto_modificadores);
    }

    LOAD;

    return 1;

}


// <resto_modificadores> 
//   | <modificador> <resto_modificadores> 
//   | ε
int resto_modificadores() {

    if (modificador()) {
        IFPROD(resto_modificadores);
    }

    VOLTA1;

    return 1;

}


// <modificador> 
//   | '[' <integer_literal> ']'
int modificador() {

    get_token();

    //IFTIS(TKAsterisco);

    // se nao for esses precisa ser o "[int]"
    IFTNOT(TKAbreColchete);
    GETNOT(TKIntegerLiteral)
    GETNOT(TKFechaColchete);

    return 1;

}


// <funcao> 
//   | <tipo> <identificador> '(' <lista_parametros> ')' <bloco_chave>
int funcao() {

    if (token.num > 1) {
        // gambi pro caso especifico da primeira chamada da produção <programa>
        get_token();
    }

    IFNPROD(tipo);

    GETNOTPRINT(TKId, "Identificador");

    GETNOTPRINT(TKAbrePar, "(");

    SAVE;

    if (!lista_parametros()) {
        LOAD;
    }

    GETNOTPRINT(TKFechaPar, ")");

    IFNPROD(bloco_chave);

    return 1;

}

// <lista_parametros> 
//   | <parametro> <resto_lista_parametros>
int lista_parametros() {

    SAVE;

    if (parametro()) {
        IFPROD(resto_lista_parametros);
    }

    LOAD;

    return 1;
}


// <resto_lista_parametros> 
//   | ',' <parametro> <resto_lista_parametros> 
//   | ε
int resto_lista_parametros() {

    get_token();

    if (TNOT(TKVirgula)) {
        VOLTA1;
        return 1;
    }

    if (parametro()) {
        IFPROD(resto_lista_parametros);
    }

    return 1;

}

// <parametro> 
//   | <tipo> <identificador>
int parametro() {

    get_token();

    IFNPROD(tipo);

    GETNOTPRINT(TKId, "identificador");

    return 1;

}


// <bloco_chave> 
//   | '{' <lista_comandos> '}'
int bloco_chave() {

    SAVE;

    GETNOT(TKAbreChave);

    IFNPROD(lista_comandos);

    GETNOTPRINT(TKFechaChave, "}");

    //POPALL;

    return 1;

}


// <lista_comandos> 
//   | <comando> <resto_lista_comandos> 
//   | ε
int lista_comandos() {

    SAVE;

    if (comando()) {
        IFPROD(resto_lista_comandos);
    }

    LOAD;

    return 1;

}

// <resto_lista_comandos> 
//   | <comando> <resto_lista_comandos> 
//   | ε
int resto_lista_comandos() {
    
    SAVE;

    if (comando()) {
        IFPROD(resto_lista_comandos);
    }

    LOAD;

    return 1;

}


// <comando> 
//   | <declaracoes> 
//   | <comando_controle> 
//   | <expressao> ';' 
//   | <bloco_chave>
int comando() {

    SAVE;

    if (expressao()) {
        GETNOTPRINT(TKPontoVirgula, ";");
        return 1;
    }
    LOAD;

    IFPROD(declaracoes);
    LOAD;

    IFPROD(comando_controle);
    LOAD;

    IFPROD(retorno);
    LOAD;

    IFPROD(bloco_chave);

    return 0;

}


// <comando_controle> 
//   | <while_loop> 
//   | <do_while_loop> 
//   | <for_loop> 
//   | <if>
int comando_controle() {

    get_token();

    SAVE;

    switch (token.tk) {

        case TKWhile: IFPROD(while_loop);    break;
        case TKDo:    IFPROD(do_while_loop); break;
        case TKFor:   IFPROD(for_loop);      break;    
        case TKIf:    IFPROD(comando_if);    break;

    default:
        IFPROD(controle_laco);
        break;
    }

    LOAD;
    return 0;

}


// <while_loop> 
//   | while '(' <expressao> ')' <comando>
int while_loop() {

    GETNOTPRINT(TKAbrePar, "(");

    IFNPROD(expressao);

    GETNOTPRINT(TKFechaPar, ")");

    IFNPROD(comando);

    return 1;

}

// <do_while_loop> 
//   | do <comando> while '(' <expressao> ')' ';'
int do_while_loop() {

    IFNPROD(comando);

    GETNOTPRINT(TKWhile, "while");

    GETNOTPRINT(TKAbrePar, "(");

    IFNPROD(expressao);

    GETNOTPRINT(TKFechaPar, ")");

    GETNOTPRINT(TKPontoVirgula, ";");

    return 1;

}

// <for_loop> 
//   | for '(' <expressao> ';' <expressao> ';' <expressao> ')' <comando>
int for_loop() {

    GETNOTPRINT(TKAbrePar, "(");

    IFNPROD(expressao);

    GETNOTPRINT(TKPontoVirgula, ";");

    IFNPROD(expressao);

    GETNOTPRINT(TKPontoVirgula, ";");

    IFNPROD(expressao);

    GETNOTPRINT(TKFechaPar, ")");

    IFNPROD(comando);

    return 1;

}

// <if> 
//   | if '(' <expressao> ')' <comando> <else>
int comando_if() {

    GETNOTPRINT(TKAbrePar, "(");

    IFNPROD(expressao);

    GETNOTPRINT(TKFechaPar, ")");

    IFNPROD(comando);

    IFNPROD(comando_else);

    return 1;    

}

// <else> 
//   | else <comando> 
//   | ε
int comando_else() {

    get_token();

    if TNOT(TKElse) {
        VOLTA1;
        return 1;
    }

    IFNPROD(comando);

    return 1;

}


// <expressao> 
//   | <expressao_atrib> <resto_expressao>
int expressao() {

    IFNPROD(expressao_atrib);

    IFNPROD(resto_expressao);

    return 1;

}


// <resto_expressao> 
//   | ',' <expressao_atrib> <resto_expressao> 
//   | ε
int resto_expressao() {

    get_token();

    if (TNOT(TKVirgula)) {
        VOLTA1;
        return 1;
    }

    IFNPROD(expressao_atrib);

    IFNPROD(resto_expressao);

    return 1;

}

// <expressao_atrib> 
//   | <identificador> = <literal> 
//   | <expressao_logica> <operador_atrib> <expressao_atrib> 
//   | <expressao_logica>
int expressao_atrib() {

    get_token();

    SAVE;

    if (TIS(TKId)) {
        get_token();
        if (operador_atrib()) {
            get_token();
            IFPROD(constante);
            VOLTA1; // não achou uma constante, tentar um id
            GETNOTPRINT(TKId, "Identificador ou literal");
            return 1;
        }
    }

    LOAD;

    IFNPROD(expressao_logica);

    get_token();

    if (!operador_atrib()) {
        VOLTA1;
        return 1; // ja acabou
    } else {
        // precisa de um assign
        IFNPROD(expressao_atrib);
    }

    return 1;

}

// <expressao_logica> 
//   | <expressao_igualdade> <resto_expressao_logica>
int expressao_logica() {

    if (expressao_igualdade()) {
        IFPROD(resto_expressao_logica);
    }

    return 0;
}


// <resto_expressao_logica> 
//   | <logical_operator> <expressao_igualdade> <resto_expressao_logica> 
//   | ε
int resto_expressao_logica() {

    SAVE;

    get_token();

    if (operador_logico()) {
        
        get_token();
       
        if (expressao_igualdade()) {

            IFPROD(resto_expressao_logica);
            
            return 0;
        }
        return 0;
    }

    LOAD;

    return 1;

}

// <expressao_igualdade> 
//   | <expressao_relacional> <resto_expressao_igualdade>
int expressao_igualdade() {

    if (expressao_relacional()) {
        IFPROD(resto_expressao_igualdade);
    }

    return 0;

}

// <resto_expressao_igualdade> 
//   | <operador_igualdade> <expressao_relacional> <resto_expressao_igualdade> 
//   | ε
int resto_expressao_igualdade() {

    SAVE;

    get_token();

    if (operador_igualdade()) {

        get_token();

        if (expressao_soma()) {

            IFPROD(resto_expressao_igualdade);
            
            return 0;
        }
        return 0;
    }

    LOAD;

    return 1;

}

// <expressao_relacional> 
//   | <expressao_soma> <resto_expressao_relacional>
int expressao_relacional() {

    if (expressao_soma()) {
        IFPROD(resto_expressao_relacional);
    }

    return 0;

}

// <resto_expressao_relacional> 
//   | <op_relacional> <expressao_soma> <resto_expressao_relacional> 
//   | ε
int resto_expressao_relacional() {

    SAVE;

    get_token();

    if (operador_relacional()) {

        get_token();

        if (expressao_soma()) {

            IFPROD(resto_expressao_relacional);
            
            return 0;
        }
        return 0;
    }

    LOAD;

    return 1;

}

// <expressao_soma> 
//   | <expressao_mult> <resto_expressao_soma>
int expressao_soma() {

    if (expressao_mult()) {
        IFPROD(resto_expressao_soma);
    }

    return 0;

}

// <resto_expressao_soma> 
//   | <operador_soma> <expressao_mult> <resto_expressao_soma> 
//   | ε
int resto_expressao_soma() {

    SAVE;
    get_token();

    if (operador_soma()) {

        get_token();

        if (expressao_mult()) {

            IFPROD(resto_expressao_soma);
            
            return 0;
        }
        return 0;
    }

    LOAD;

    return 1;

}

// <expressao_mult> 
//   | <expressao_unaria> <resto_expressao_mult>
int expressao_mult() {

    if (expressao_unaria()) {
        IFPROD(resto_expressao_mult);
    }

    return 0;

}


// <resto_expressao_mult> 
//   | <operador_mult> <expressao_unaria> <resto_expressao_mult> 
//   | ε
int resto_expressao_mult() {

    SAVE;

    if (operador_mult()) {

        get_token();

        if (expressao_unaria()) {

            IFPROD(resto_expressao_mult);

            return 0;
        }
        return 0;
    }

    LOAD;

    return 1;

}

// <expressao_unaria> 
//   | <expressao_primaria> ('++' | '--' | ε)
//   | '-' <expressao_primaria> 
//   | <operador_unario> <expressao_unaria>
int expressao_unaria() {

    //get_token();

    SAVE;

//   ---> '-' <expressao_primaria> 
    if (TIS(TKMenos)) {
        get_token();
    }

//   ---> <expressao_primaria> 
    if (expressao_primaria()) {
        // operador posfixado
        get_token();
        if (TNOT(TKDuploMais) && TNOT(TKDuploMenos)) {
            VOLTA1;
        }
        return 1;
    }


    LOAD;

//   | <operador_unario> <expressao_unaria>
    if (operador_unario()) {
        IFPROD(expressao_unaria);
    }

    return 0;

}


// <expressao_primaria> 
//   | <identificador> 
//   | <constante> 
//   | <chamada_funcao>
//   | '(' <expressao> ')' 
int expressao_primaria() {

    SAVE;

    IFPROD(constante);

    LOAD;

    IFPROD(chamada_funcao);

    LOAD;

    IFTIS(TKId);

    LOAD;

    IFTNOT(TKAbrePar);

    IFNPROD(expressao);

    GETNOTPRINT(TKFechaPar, ")");

    return 1;

}



// <constante> 
//   | <integer_literal> 
//   | <float_literal> 
//   | <char_literal>
//   | <string>
int constante() {

    if (TNOT(TKIntegerLiteral) && TNOT(TKFloatLiteral) && TNOT(TKString)) {
        SAVE;
        if (!char_literal()) {
            LOAD;
            return 0;
        }
    }

    return 1;

}

int char_literal() {

    IFTNOT(TKAspasSimples);

    // o lexico nao diferencia identificadores de chars iirc
    GETNOTPRINT(TKId, "Char literal");

    GETNOTPRINT(TKAspasSimples, "'");

    return 1;

}


// <chamada_funcao> 
//   | <identificador> '(' <argumentos> ')'
int chamada_funcao() {

    IFTNOT(TKId);

    GETNOT(TKAbrePar);

    IFNPROD(argumentos);

    GETNOTPRINT(TKFechaPar, ")");

    return 1;

}



// <argumentos> 
//   | <expressao> <resto_argumentos>
int argumentos() {

    SAVE;

    if (expressao()) {
        IFPROD(resto_argumentos);
    }

    LOAD;

    return 0;
    

}


// <resto_argumentos> 
//   | ',' <expressao> <resto_argumentos> 
//   | ε
int resto_argumentos() {

    get_token();

    if (TNOT(TKVirgula)) {
        VOLTA1;
        return 1;
    }

    if (expressao()) {
        IFPROD(resto_argumentos);
    }

    return 1;


}


// <operador_atrib> 
//   | = 
//   | += 
//   | -= 
//   | *= 
//   | /= 
//   | %= 
//   | >>= 
//   | <<=
int operador_atrib() {

    if (TNOT(TKAtribuicao) && TNOT(TKAtribMais)  && TNOT(TKAtribMenos) && 
        TNOT(TKAtribDiv)   && TNOT(TKAtribVezes) && TNOT(TKAtribVezes)) {
            return 0;
        }

    return 1;

}



// <operador_logico> 
//   | && 
//   | ||
//   | !
int operador_logico() {

    return (TIS(TKAnd) || TIS(TKOr) || TIS(TKNot));

}



// <operador_igualdade> 
//   | == 
//   | !=
int operador_igualdade() {

    return (TIS(TKIgual) || TIS(TKNaoIgual));

}


// <operador_relacional> 
//   | > 
//   | < 
//   | >= 
//   | <=
int operador_relacional() {

    return (TIS(TKMaior) || TIS(TKMenor) || TIS(TKMaiorOuIgual) || TIS(TKMenorOuIgual));

}


// <operador_soma> 
//   | + 
//   | -
int operador_soma() {

    return (TIS(TKMenos) || TIS(TKMais));

}

// <operador_mult> 
//   | * 
//   | / 
//   | %
int operador_mult() {

    return (TIS(TKAsterisco) || TIS(TKDividir) || TIS(TKResto));

}


// <operador_unario> 
//   | & 
//   | |
//   | ^ 
//   | ~ 
//   | << 
//   | >>
int operador_unario() {

    return (TIS(TKBitwiseAnd) || TIS(TKBitwiseOr) || TIS(TKBitwiseXor) || TIS(TKBitwiseNot) || TIS(TKShiftLeft) || TIS(TKShiftRight));

}

// return <expressao> ;
int retorno() {

    get_token();

    IFTNOT(TKReturn);

    SAVE;
    
    if (!expressao()) {
        LOAD;
    }

    GETNOTPRINT(TKPontoVirgula, ";");

    return 1;

}

// <break|continue> ;
int controle_laco() {

    if (TIS(TKContinue) || TIS(TKBreak)) {
        GETNOTPRINT(TKPontoVirgula, ";");
        return 1;
    }

    return 0;

    

}

int main(int argc, char **argv) {

	//validar os argumentos de entrada
	if (argc != 2) {
		printf("%s <nome do arquivo>\n", argv[0]);
		exit(0);
	}

    // inicializar o lexico com o nome do arquivo de entrada
	init_lex(argv[1]);


    if (programa() && !erro) {
        printf("Reconheceu OK\n");
    } else {
        printf("Erro\n");
    }

    fechar_lex();

    return 0;

}