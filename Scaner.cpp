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
		lex[0] = '0';
		// Проверка на шестнадцатеричную константу
		if (text[uk + 1] == 'x') {
			lex[1] = 'x';
			uk += 2;
			i = 2;

			// Считываем шестнадцатеричные цифры
			while ((isdigit(text[uk]) || (text[uk] >= 'a' && text[uk] <= 'f') || (text[uk] >= 'A' && text[uk] <= 'F')) && i < maxHexLex - 1) {
				lex[i++] = text[uk++];
			}

			// Проверка на превышение максимальной длины
			if (i == maxHexLex - 1 && (isdigit(text[uk]) || (text[uk] >= 'a' && text[uk] <= 'f') || (text[uk] >= 'A' && text[uk] <= 'F'))) {
				while (isdigit(text[uk]) || isalpha(text[uk]) ||  (text[uk] >= 'a' && text[uk] <= 'f') || (text[uk] >= 'A' && text[uk] <= 'F'))
				{
					lex[i++] = text[uk++];
				}
				lex[i] = '\0';
				PrintError("Шестнадцатеричная константа превышает максимальную длину лексемы", lex);
				return TError;
			}

			// Проверка на суффикс L для длинной шестнадцатеричной константы
			if (text[uk] == 'L') {
				lex[i++] = text[uk++];  // Добавляем 'L' к lex
				lex[i] = '\0';

				if (isalpha(text[uk])) { // Проверка на недопустимый символ в конце числа
					lex[i++] = text[uk++];
					lex[i] = '\0';
					PrintError("Недопустимая структура константы: ", lex);
					return TError;
				}
				return TCLongHex;  // Суффикс L означает длинную шестнадцатеричную константу
			}
			if (isalpha(text[uk])) { // Проверка на недопустимый символ в конце числа
				lex[i++] = text[uk++];
				lex[i] = '\0';
				PrintError("Недопустимая структура константы: ", lex);
				return TError;
			}
			lex[i] = '\0';
			return TConstHex;  // Обычная шестнадцатеричная константа
		}

		// Обработка десятичных чисел, начинающихся с 0 (например, "0123")
		i = 0;
		lex[i++] = text[uk++];  // Добавляем начальный '0' к lex

		// Считываем оставшиеся десятичные цифры
		while (isdigit(text[uk]) && i < maxDecLex - 1) {
			lex[i++] = text[uk++];
		}

		// Проверка на превышение максимальной длины
		if (i == maxDecLex - 1 && isdigit(text[uk])) {
			while (isdigit(text[uk]) || isalpha(text[uk]))
			{
				lex[i++] = text[uk++];
			}
			lex[i] = '\0';
			PrintError("Десятичная константа превышает максимальную длину лексемы", lex);
			return TError;
		}

		// Проверка на суффикс L для длинной десятичной константы
		if (text[uk] == 'L') {
			lex[i++] = text[uk++];  // Добавляем 'L' к lex
			lex[i] = '\0';

			if (isalpha(text[uk])) { // Проверка на недопустимый символ в конце числа
				lex[i++] = text[uk++];
				lex[i] = '\0';
				PrintError("Недопустимая структура константы: ", lex);
				return TError;
			}
			lex[i] = '\0';
			return TCLongInt;  // Суффикс L означает длинную десятичную константу
		}

		// Проверка на недопустимый символ после чисел
		if (isalpha(text[uk])) {
			lex[i++] = text[uk++];
			lex[i] = '\0';
			PrintError("Недопустимая структура константы: ", lex);
			return TError;
		}
		lex[i] = '\0';
		return TConstInt;  // Обычное десятичное число, начинающееся с 0 (например, "0123")
	}

	// Обработка десятичных констант, включая случаи, когда число начинается с цифры от 1 до 9
	if (isdigit(text[uk])) {
		i = 0;

		// Считываем десятичные цифры
		while (isdigit(text[uk]) && i < maxDecLex - 1) {
			lex[i++] = text[uk++];
		}
		
		// Проверка на превышение максимальной длины
		if (i == maxDecLex - 1 && isdigit(text[uk])) {
			while (isdigit(text[uk]) || isalpha(text[uk]))
			{
				lex[i++] = text[uk++];
			}
			lex[i] = '\0';
			PrintError("Десятичная константа превышает максимальную длину лексемы", lex);
			return TError;
		}

		// Проверка на суффикс L для длинной десятичной константы
		if (text[uk] == 'L') {
			lex[i++] = text[uk++];  // Добавляем 'L' к lex
			lex[i] = '\0';

			if (isalpha(text[uk])) { // Проверка на недопустимый символ в конце числа
				lex[i++] = text[uk++];
				lex[i] = '\0';
				PrintError("Недопустимая структура константы", lex);
				return TError;
			}
			return TCLongInt;  // Суффикс L означает длинную десятичную константу
		}

		if (isalpha(text[uk])) { // Проверка на недопустимый символ в конце числа
			lex[i++] = text[uk++];
			lex[i] = '\0';
			PrintError("Недопустимая структура константы", lex);
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
