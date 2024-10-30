#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxText 100000			// Максимальная длина текста
#define maxLex 100				// Максимальная длина лексемы
#define maxKeyword 16			// Максимальная длина ключевого слова
#define maxDecLex 10			// Максимальная длина числа
#define maxHexLex 8
typedef char Type_mod[maxText]; // Текст
typedef char Type_lex[maxLex];  // Лексема

/**
* Ключевые слова
*/
#define TInt 1
#define TShort 2
#define TLong 3
#define TLongLong 4
#define TFor 5
#define TReturn 6
#define TMain 7

/**
* Тип лексем
*/
#define TId 20

/**
* Константы
*/
#define TConstInt 30
#define TConstHex 31
#define TCLongInt 32
#define TCLongHex 33

/**
* Знаки операций
*/
#define TPlus 40			// +
#define TMinus 41			// -
#define TMul 42				// *
#define TDiv 43				// /
#define TMod 44				// %
#define TAssign 51			// =
#define TEq 45				// ==
#define TNotEq 46			// !=
#define TLess 47			// <
#define TLessEq 48		// <=
#define TGreater 49			// >
#define TGreaterEq 50		// >=

/**
* Специальные знаки
*/
#define TSemi 60		// ;
#define TComma 61		// ,
#define TLBracket 62	// (
#define TRBracket 63	// )
#define TLBrace 64		// {
#define TRBrace 65		// }

/**
* Дополнительно
*/
#define TEnd 100
#define TError 200