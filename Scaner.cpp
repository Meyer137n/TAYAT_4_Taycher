#define _CRT_SECURE_NO_WARNINGS

#include "Scaner.h"
#include <fstream>
#include <sstream>
#include <iostream>

Type_lex keyword[maxKeyword] =
{
	"int", "short", "long", "long long", "for", "main"
};

int indexKeyword[maxKeyword] =
{
	TInt, TShort, TLong, TLongLong, TFor, TMain, TReturn
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

	// ����� ���������
	if (text[uk] == '\0')
	{
		lex[i++] = '#';
		lex[i] = '\0';
		return TEnd;
	}

	// ������������ � ������������� �����������
	if (text[uk] == '/')
	{
		uk++;
		if (text[uk] == '/')
		{
			std::cout << "��������� ������������ �����������" << std::endl;
			uk++;
			while (text[uk] != '\n' && text[uk] != '\0')
				uk++;
			goto start;
		}
		else
		{
			// �������
			uk++;
			lex[i++] = '/';
			lex[i++] = '\0';
			return TDiv;
		}
	}

	// ����������������� ���������
	if (text[uk] == '0' && (text[uk + 1] == 'x' || text[uk + 1] == 'X')) {
		// ���������� ������� 0x ��� 0X
		uk += 2;
		i = 0;

		// ��������� ����������������� �����
		while ((isdigit(text[uk]) || (text[uk] >= 'a' && text[uk] <= 'f') || (text[uk] >= 'A' && text[uk] <= 'F')) && i < maxHexLex - 1)
		{ // ��������� �������� �����
			lex[i++] = text[uk++];
		}
		lex[i] = '\0';

		// �������� �� ���������� ������������ �����
		if (i == maxHexLex - 1 && (isdigit(text[uk]) || (text[uk] >= 'a' && text[uk] <= 'f') || (text[uk] >= 'A' && text[uk] <= 'F')))
		{
			while (isdigit(text[uk]) || (text[uk] >= 'a' && text[uk] <= 'f') || (text[uk] >= 'A' && text[uk] <= 'F'))
				uk++;
			PrintError("����������������� ��������� ��������� ������������ ����� �������", lex);
			return TError;
		}

		// �������� �� ������� ����������������� ��������� � ���������� L ��� LL
		if (text[uk] == 'L') {
			uk++;
			if (text[uk] == 'L') {
				uk++;
				return TCLongHex;  // ������� LL �������� ������� ����������������� ���������
			}
			return TCLongHex;  // ������� L �������� ������� ����������������� ���������
		}

		return TConstHex;  // ������� ����������������� ���������
	}

	// ���������� ���������
	if (isdigit(text[uk])) {
		while (isdigit(text[uk]) && i < maxDecLex - 1)
		{ // ��������� �������� �����
			lex[i++] = text[uk++];
		}
		lex[i] = '\0';

		// �������� �� ���������� ������������ �����
		if (i == maxDecLex - 1 && isdigit(text[uk]))
		{
			while (isdigit(text[uk]))
				uk++;
			PrintError("���������� ��������� ��������� ������������ ����� �������", lex);
			return TError;
		}

		// �������� �� ������� ���������� ��������� � ���������� L ��� LL
		if (text[uk] == 'L') {
			uk++;
			if (text[uk] == 'L') {
				uk++;
				return TCLongInt;  // ������� LL �������� ������� ���������� ���������
			}
			return TCLongInt;  // ������� L �������� ������� ���������� ���������
		}

		return TConstInt;  // ������� ���������� ���������
	}

	// ��������������
	if (isalpha(text[uk]) || text[uk] == '_')
	{
		while ((isalnum(text[uk]) || text[uk] == '_') && i < maxLex - 1)
		{ // ��������� �������� �����
			lex[i++] = text[uk++];
		}
		lex[i] = '\0';
		if (i == maxLex - 1 && (isalnum(text[uk]) || text[uk] == '_'))
		{
			while (isalnum(text[uk]) || text[uk] == '_')
				uk++;
			PrintError("������������� ��������� ������������ ����� �������", lex);
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

	// ���������
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
		// �� ����������� �������� ��
		else
		{
			lex[i] = '\0';
			PrintError("����������� �����", lex);
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
		PrintError("����������� ������ �", lex);
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
		std::cout << "\n_____________________________________________________________________________________________________\n\n\n";
	}
	else
	{
		PrintError("������ �������� �����!", filename);
		return;
	}
}

Scaner::Scaner(const char* filename)
{
	GetData(filename);
	PutUK(0);
}
