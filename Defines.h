#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxText 100000			// ������������ ����� ������
#define maxLex 100				// ������������ ����� �������
#define maxKeyword 16			// ������������ ����� ��������� �����
#define maxDecLex 10			// ������������ ����� �����
#define maxHexLex 8
typedef char Type_mod[maxText]; // �����
typedef char Type_lex[maxLex];  // �������

/**
* �������� �����
*/
#define TInt 1
#define TShort 2
#define TLong 3
#define TLongLong 4
#define TFor 5
#define TReturn 6
#define TMain 7

/**
* ��� ������
*/
#define TId 20

/**
* ���������
*/
#define TConstInt 30
#define TConstHex 31
#define TCLongInt 32
#define TCLongHex 33

/**
* ����� ��������
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
* ����������� �����
*/
#define TSemi 60		// ;
#define TComma 61		// ,
#define TLBracket 62	// (
#define TRBracket 63	// )
#define TLBrace 64		// {
#define TRBrace 65		// }

/**
* �������������
*/
#define TEnd 100
#define TError 200