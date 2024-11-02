#define _CRT_SECURE_NO_WARNINGS

#include "Scaner.h"
#include <fstream>
#include <sstream>
#include <iostream>

Type_lex keyword[maxKeyword] =
{
	"int", "short", "long", "for", "main", "return", "const"
};

int indexKeyword[maxKeyword] =
{
	TInt, TShort, TLong, TFor, TMain, TReturn, TConst
};

void Scaner::PutUK(int uk)
{
	if (uk >= 0 && uk <= maxText)
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

int Scaner::UseScaner(Type_lex lex)
{
	int i = 0;
	lex[0] = '\0';
start:
	while (text[uk] == ' ' || text[uk] == '\t' || text[uk] == '\n')
		uk++;
	i = 0;

	// Конец программы
	if (text[uk] == '\0')
	{
		lex[i++] = '#';
		lex[i] = '\0';
		return TEnd;
	}

	// Однострочные и многострочные комментарии
	if (text[uk] == '/')
	{
		uk++;
		if (text[uk] == '/')
		{
			std::cout << "Обнаружен однострочный комментарий" << std::endl;
			uk++;
			while (text[uk] != '\n' && text[uk] != '\0')
				uk++;
			goto start;
		}
		else
		{
			// Деление
			uk++;
			lex[i++] = '/';
			lex[i++] = '\0';
			return TDiv;
		}
	}

	// Обработка шестнадцатеричных констант и чисел, начинающихся с 0
	if (text[uk] == '0') {
		i = 0;
		lex[i++] = text[uk++];


		// Проверка на шестнадцатеричную константу
		if (text[uk] == 'x') {
			lex[i++] = text[uk++];

			// Считываем шестнадцатеричные цифры
			while (isdigit(text[uk]) || (text[uk] >= 'a' && text[uk] <= 'f') || (text[uk] >= 'A' && text[uk] <= 'F')) 
			{
				lex[i++] = text[uk++];
			}

			if (text[uk] == 'L')
			{
				lex[i] = text[uk];  // Добавляем 'L' к lex
			}

			// Проверка на суффикс L для длинной десятичной константы
			if (text[uk] == 'L')
			{
				uk++;
				i++;
				// Проверка на максимальную длину для длинной шестнадцатеричной константы (8 символов)
				if (i > maxHexLex + 3) {
					lex[i] = '\0';
					PrintError("Длинная десятичная константа превышает допустимую длину", lex);
					return TError;
				}
				lex[i] = '\0';
				return TCLongHex;
			}

			// Проверка на превышение длины обычной шестнадцатеричной константы
			if (i > maxHexLex+2)
			{
				lex[i] = '\0';
				PrintError("Десятичная константа превышает допустимую длину", lex);
				return TError;
			}

			lex[i] = '\0';
			return TConstHex;  // Обычная шестнадцатеричная константа
		}

		// Обработка десятичных чисел, начинающихся с 0 (например, "0123")
		while (isdigit(text[uk])) // maxDecLex - максимальная длина для обычных чисел
		{
			lex[i++] = text[uk++];
		}

		if (text[uk] == 'L')
		{
			lex[i] = text[uk];  // Добавляем 'L' к lex
		}

		// Проверка на суффикс L для длинной десятичной константы
		if (text[uk] == 'L')
		{
			uk++;
			i++;
			// Проверка на максимальную длину для длинной десятичной константы (10 символов)
			if (i > maxDecLex + 1) {
				lex[i] = '\0';
				PrintError("Длинная десятичная константа превышает допустимую длину", lex);
				return TError;
			}
			lex[i] = '\0';
			return TCLongInt;
		}

		// Проверка на превышение длины обычной константы
		if (i > maxDecLex)
		{
			lex[i] = '\0';
			PrintError("Десятичная константа превышает допустимую длину", lex);
			return TError;
		}
		lex[i] = '\0';
		return TConstInt;  // Обычное десятичное число, начинающееся с 0 (например, "0123")
	}

	// Обработка десятичных констант, начинающихся с 1...9
	if (text[uk] >= '1' && text[uk] <= '9') 
	{
		i = 0;

		// Считываем десятичные цифры
		while (isdigit(text[uk])) // maxDecLex - максимальная длина для обычных чисел
		{  
			lex[i++] = text[uk++];
		}

		if (text[uk] == 'L')
		{
			lex[i] = text[uk];  // Добавляем 'L' к lex
		}

		// Проверка на суффикс L для длинной десятичной константы
		if (text[uk] == 'L')
		{
			uk++;
			i++;
			// Проверка на максимальную длину для длинной десятичной константы (10 символов)
			if (i > maxDecLex + 1) {
				lex[i] = '\0';
				PrintError("Длинная десятичная константа превышает допустимую длину", lex);
				return TError;
			}
			lex[i] = '\0';
			return TCLongInt;
		}

		// Проверка на превышение длины обычной константы
		if (i > maxDecLex)
		{
			lex[i] = '\0';
			PrintError("Десятичная константа превышает допустимую длину", lex);
			return TError;
		}
	
		lex[i] = '\0';
		return TConstInt;  // Обычная десятичная константа
	}
	
	// Идентификаторы
	if (isalpha(text[uk]) || text[uk] == '_')
	{
		while ((isalnum(text[uk]) || text[uk] == '_') && i < maxLex - 1)
		{ // Добавлена проверка длины
			lex[i++] = text[uk++];
		}
		lex[i] = '\0';
		if (i == maxLex - 1 && (isalnum(text[uk]) || text[uk] == '_'))
		{
			while (isalnum(text[uk]) || text[uk] == '_')
				uk++;
			PrintError("Идентификатор превышает максимальную длину лексемы", lex);
			return TError;
		}
		for (int j = 0; j < maxKeyword; j++)
		{
			if (strcmp(lex, keyword[j]) == 0)
			{
				return indexKeyword[j];
			}
		}
		return TId;
	}

	// Знаки операций
	switch (text[uk])
	{
	case ';': uk++; lex[i++] = ';'; lex[i] = '\0'; return TSemi;
	case ',': uk++; lex[i++] = ','; lex[i] = '\0'; return TComma;
	case '(': uk++; lex[i++] = '('; lex[i] = '\0'; return TLBracket;
	case ')': uk++; lex[i++] = ')'; lex[i] = '\0'; return TRBracket;
	case '{': uk++; lex[i++] = '{'; lex[i] = '\0'; return TLBrace;
	case '}': uk++; lex[i++] = '}'; lex[i] = '\0'; return TRBrace;
	case '+': uk++; lex[i++] = '+'; lex[i] = '\0'; return TPlus;
	case '-': uk++; lex[i++] = '-'; lex[i] = '\0'; return TMinus;
	case '*': uk++; lex[i++] = '*'; lex[i] = '\0'; return TMul;
	case '/': uk++; lex[i++] = '/'; lex[i] = '\0'; return TDiv;
	case '%': uk++; lex[i++] = '%'; lex[i] = '\0'; return TMod;
	case '=':
		uk++;
		lex[i++] = '=';
		if (text[uk] == '=')
		{
			uk++;
			lex[i++] = '=';
			lex[i] = '\0';
			return TEq;
		}
		else
		{
			lex[i] = '\0';
			return TAssign;
		}
	case '!':
		uk++;
		lex[i++] = '!';
		if (text[uk] == '=')
		{
			uk++;
			lex[i++] = '=';
			lex[i] = '\0';
			return TNotEq;
		}
		else
		{
			lex[i] = '\0';
			PrintError("Неожиданный токен", lex);
			return TError;
		}
	case '>':
		uk++;
		lex[i++] = '>';
		if (text[uk] == '=')
		{
			uk++;
			lex[i++] = '=';
			lex[i] = '\0';
			return TGreaterEq;
		}
		else
		{
			lex[i] = '\0';
			return TGreater;
		}
	case '<':
		uk++;
		lex[i++] = '<';
		if (text[uk] == '=')
		{
			uk++;
			lex[i++] = '=';
			lex[i] = '\0';
			return TLessEq;
		}
		else
		{
			lex[i] = '\0';
			return TLess;
		}
	default:
		lex[i++] = text[uk];
		lex[i] = '\0';
		PrintError("Лексическая ошибка в", lex);
		uk++;
		return TError;
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
		std::cout << "\n........................................................................................................................\n\n\n";
	}
	else
	{
		PrintError("Ошибка открытия файла!", filename);
		return;
	}
}

Scaner::Scaner(const char* filename)
{
	GetData(filename);
	PutUK(0);
}
