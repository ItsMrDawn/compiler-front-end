
#include "defs.h"

// definições comuns usadas pelo lex

const char* str_tokens[] = {
	// tipos
	"TKInt",
	"TKFloat",
	"TKChar",
	"TKDouble",
	"TKVoid",
	"TKLong",
	"TKUnsigned",

	// palavras-chave
	"TKStruct",
	"TKWhile",
	"TKFor",
	"TKDo",
	"TKIf",
	"TKElse",
	"TKSwitch",
	"TKCase",
	"TKDefault",
	"TKReturn",
	"TKContinue",
	"TKBreak",

	// identificadores e literais
	"TKId",
	"TKIntegerLiteral",
	"TKFloatLiteral",
	"TKCharLiteral",
	"TKString",

	// operadores
	"TKMais",
	"TKMenos",
	"TKVezes",
	"TKDividir",
	"TKResto",
	"TKBitwiseAnd",
	"TKBitwiseOr",
	"TKBitwiseXor",
	"TKShiftRight",
	"TKShiftLeft",
	"TKBitwiseNot",

	// relacionais
	"TKMaior",
	"TKMenor",
	"TKMaiorOuIgual",
	"TKMenorOuIgual",
	"TKIgual",
	"TKNaoIgual",

	// diabos
	"TKDuploMais",
	"TKDuploMenos",
	"TKAsterisco",
	"TKSeta",
	"TKComentario",
	"TKAspasSimples",
	"TKAspasDuplas",

	// atrib
	"TKAtribuicao",
	"TKAtribMais",
	"TKAtribMenos",
	"TKAtribVezes",
	"TKAtribDiv",

	// lógicos
	"TKAnd",
	"TKOr",
	"TKNot",

	// delimitadores
	"TKVirgula",
	"TKDoisPontos",
	"TKPontoVirgula",
	"TKPonto",
	"TKAbrePar",
	"TKFechaPar",
	"TKAbreColchete",
	"TKFechaColchete",
	"TKAbreChave",
	"TKFechaChave",

	// outros
	"TKEOF",
	"TKErro"
};

struct pal_res lista_pal[] = {{"void", TKVoid},
							  {"int", TKInt},
							  {"float", TKFloat},
							  {"char", TKChar},
							  {"double", TKDouble},
							  {"long", TKLong},
							  {"unsigned", TKUnsigned},
							  {"if", TKIf},
							  {"else", TKElse},
							  {"struct", TKStruct},
							  {"for", TKFor},
							  {"do", TKDo},
							  {"while", TKWhile},
							  {"switch", TKSwitch},
							  {"case", TKCase},
							  {"default", TKDefault},
							  {"return", TKReturn},
							  {"continue", TKContinue},
							  {"break", TKBreak},
							  {"fimtabela", TKId}
};

struct ops lista_ops[] = {{">>", TKShiftRight}, //com dois caracteres
						  {"<<", TKShiftLeft},
						  {">=", TKMaiorOuIgual},
						  {"<=", TKMenorOuIgual},
						  {"==", TKIgual},
						  {"!=", TKNaoIgual},
						  {"+=", TKAtribMais},
						  {"-=", TKAtribMenos},
						  {"*=", TKAtribVezes},
						  {"/=", TKAtribDiv},
						  {"&&", TKAnd},
						  {"||", TKOr},
						  {"->", TKSeta},
						  {"++", TKDuploMais},
						  {"--", TKDuploMenos},
						  {"//", TKComentario},
						  {"$$", TKId},

						  {"* ", TKAsterisco}, //com um caracter
	  					  {"+ ", TKMais},
						  {"- ", TKMenos},
						  {"/ ", TKDividir},
						  {"% ", TKResto},
						  {"& ", TKBitwiseAnd},
						  {"| ", TKBitwiseOr},
						  {"~ ", TKBitwiseNot},
						  {"^ ", TKBitwiseXor},
						  {"> ", TKMaior},
						  {"< ", TKMenor},
						  {"= ", TKAtribuicao},
						  {"! ", TKNot},
						  {"\" ", TKString},
						  {"' ", TKAspasSimples},
						  {", ", TKVirgula},
						  {"; ", TKPontoVirgula},
						  {": ", TKDoisPontos},
						  {". ", TKPonto},
						  {"[ ", TKAbreColchete},
						  {"] ", TKFechaColchete},
						  {"( ", TKAbrePar},
						  {") ", TKFechaPar},
						  {"{ ", TKAbreChave},
						  {"} ", TKFechaChave},
					  	  {"$ ", TKId}
};
