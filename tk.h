#ifndef TK_H
#define TK_H

enum enum_tk {
	// tipos
	TKInt,
	TKFloat,
	TKChar,
	TKDouble,
	TKVoid,
	TKLong,
	TKUnsigned,

	// palavras-chave
	TKStruct,
	TKWhile,
	TKFor,
	TKDo,
	TKIf,
	TKElse,
	TKSwitch,
	TKCase,
	TKDefault,
	TKReturn,
	TKContinue,
	TKBreak,

	// identificadores e literais
	TKId,
	TKIntegerLiteral,
	TKFloatLiteral,
	TKCharLiteral,
	TKString,

	// operadores
	TKMais,
	TKMenos,
	TKVezes,
	TKDividir,
	TKResto,
	TKBitwiseAnd,
	TKBitwiseOr,
	TKBitwiseXor,
	TKShiftRight,
	TKShiftLeft,
	TKBitwiseNot,

	// relacionais
	TKMaior,
	TKMenor,
	TKMaiorOuIgual,
	TKMenorOuIgual,
	TKIgual,
	TKNaoIgual,

	// diabos
	TKDuploMais,
	TKDuploMenos,
	TKAsterisco,
	TKSeta,
	TKComentario,
	TKAspasSimples,
	TKAspasDuplas,

	// atrib
	TKAtribuicao,
	TKAtribMais,
	TKAtribMenos,
	TKAtribVezes,
	TKAtribDiv,


	// lógicos
	TKAnd,
	TKOr,
	TKNot,

	// delimitadores
	TKVirgula,
	TKDoisPontos,
	TKPontoVirgula,
	TKPonto,
	TKAbrePar,
	TKFechaPar,
	TKAbreColchete,
	TKFechaColchete,
	TKAbreChave,
	TKFechaChave,

	// outros
	TKEOF,
	TKErro,

	NUM_TOKENS
};

#endif
