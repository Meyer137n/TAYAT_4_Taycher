#define _CRT_SECURE_NO_WARNINGS

#include "Scaner.h"
#include <fstream>
#include <sstream>
#include <iostream>

type_lex keyword[maxKeyword] =
{
	"int", "short", "long", "float" "char", "while", "main"
};

int indexKeyword[maxKeyword] =
{
	typeInt, typeShort, typeLong, typeFloat, typeWhile, typeMain
};

void Scaner::PutUK(int uk)
{
	if(uk >= 0 && uk <= maxText)
		this->uk = uk;
}

int Scaner::GetUK()
{
    return uk;
}

void Scaner::PrintError(const char* error, const char* text)
{
	if (text[0] == '\0')
		std::cout << error << std::endl;
	else
		std::cout << error << " " << text << std::endl;
}

int Scaner::UseScaner(type_lex lex)
{
	int i = 0;
	lex[0] = '\0';
start:
	while (text[uk] == ' ' || text[uk] == '\t' || text[uk] == '\n')
		uk++;
	i = 0;

	// End of program
	if (text[uk] == '\0')
	{
		lex[i++] = '#';
		lex[i] = '\0';
		return typeEnd;
	}

	// Comments one-line and multi-line
	if (text[uk] == '/')
	{
		uk++;
		if (text[uk] == '/')
		{
			std::cout << "Encountered comment line" << std::endl;
			uk++;
			while (text[uk] != '\n' && text[uk] != '\0')
				uk++;
			goto start;
		}
		else
		{
			// Division
			uk++;
			lex[i++] = '/';
			lex[i++] = '\0';
			return typeDiv;
		}
	}

	// Decimal constants
	if (isdigit(text[uk])) {
		while (isdigit(text[uk]) && i < maxDecLex - 1)
		{ // Added length check
			lex[i++] = text[uk++];
		}
		lex[i] = '\0';
		if (i == maxDecLex - 1 && isdigit(text[uk]))
		{
			while (isdigit(text[uk]))
				uk++;
			PrintError("Decimal constant exceeds maximum lexeme length", lex);
			return typeError;
		}
		return typeInt;
	}

	// Identifiers
	if (isalpha(text[uk]) || text[uk] == '_')
	{
		while ((isalnum(text[uk]) || text[uk] == '_') && i < maxLex - 1)
		{ // Added length check
			lex[i++] = text[uk++];
		}
		lex[i] = '\0';
		if (i == maxLex - 1 && (isalnum(text[uk]) || text[uk] == '_'))
		{
			while (isalnum(text[uk]) || text[uk] == '_')
				uk++;
			PrintError("Identifier exceeds maximum lexeme length", lex);
			return typeError;
		}

		for (int j = 0; j < maxKeyword; j++)
		{
			if (strcmp(lex, keyword[j]) == 0)
			{
				return indexKeyword[j];
			}
		}
		return typeId;
	}

	// Operators
	switch (text[uk])
	{
	case ',': uk++; lex[i++] = ','; lex[i] = '\0'; return typeComma;
	case ';': uk++; lex[i++] = ';'; lex[i] = '\0'; return typeSemicolon;
	case '(': uk++; lex[i++] = '('; lex[i] = '\0'; return typeLeftBracket;
	case ')': uk++; lex[i++] = ')'; lex[i] = '\0'; return typeRightBracket;
	case '{': uk++; lex[i++] = '{'; lex[i] = '\0'; return typeLeftBrace;
	case '}': uk++; lex[i++] = '}'; lex[i] = '\0'; return typeRightBrace;
	case '[': uk++; lex[i++] = '['; lex[i] = '\0'; return typeLeftBracket;
	case ']': uk++; lex[i++] = ']'; lex[i] = '\0'; return typeRightBracket;
	case '+': uk++; lex[i++] = '+'; lex[i] = '\0'; return typePlus;
	case '-': uk++; lex[i++] = '-'; lex[i] = '\0'; return typeMinus;
	case '*': uk++; lex[i++] = '*'; lex[i] = '\0'; return typeMul;
	case '%': uk++; lex[i++] = '%'; lex[i] = '\0'; return typeMod;
	case '=':
		uk++;
		lex[i++] = '=';
		if (text[uk] == '=')
		{
			uk++;
			lex[i++] = '=';
			lex[i] = '\0';
			return typeEq;
		}
		else
		{
			lex[i] = '\0';
			return typeEval;
		}
	case '!':
		uk++;
		lex[i++] = '!';
		if (text[uk] == '=')
		{
			uk++;
			lex[i++] = '=';
			lex[i] = '\0';
			return typeUnEq;
		}
		// Not implementing NOT operation
		else
		{
			lex[i] = '\0';
			PrintError("Unexpected token", lex);
			return typeError;
		}
	case '>':
		uk++;
		lex[i++] = '>';
		if (text[uk] == '=')
		{
			uk++;
			lex[i++] = '=';
			lex[i] = '\0';
			return typeMoreOrEq;
		}
		else
		{
			lex[i] = '\0';
			return typeMore;
		}
	case '<':
		uk++;
		lex[i++] = '<';
		if (text[uk] == '=')
		{
			uk++;
			lex[i++] = '=';
			lex[i] = '\0';
			return typeLessOrEq;
		}
		else
		{
			lex[i] = '\0';
			return typeMore;
		}
	default:
		lex[i++] = text[uk];
		lex[i] = '\0';
		PrintError("Lexical error at", lex);
		uk++;
		return typeError;
	}
}

void Scaner::GetData(const char* filename)
{
	std::ifstream file(filename);
	if (file.is_open())
	{
		std::stringstream buffer;
		buffer << file.rdbuf();

		strncpy(text, buffer.str().c_str(), maxText - 1);
		text[maxText - 1] = '\0';

		std::cout << text << std::endl;
		std::cout << "_____________________________________________________________________________________________________	" << std::endl;
	}
	else
	{
		PrintError("Error opening file!", filename);
		return;
	}
}

Scaner::Scaner(const char* filename)
{
    GetData(filename);
    PutUK(0);
}
