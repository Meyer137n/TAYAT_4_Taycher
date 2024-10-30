#pragma once
#include "Defines.h"

class Scaner
{

private:
	Type_mod text; // Исходный текст
	int uk;		   // Указатель текущей позиции в исходном тексте

public:
	void PutUK(int uk);				// Установить указатель текущей позиции в исходном тексте
	int GetUK();					// Получить указатель текущей позиции в исходном тексте
	void PrintError(const char* error, const char* text);	// Вывод ошибки
	int UseScaner(Type_lex lex);								// Логика работы сканера
	void GetData(const char* filename);							// Получение информации из файла
	Scaner(const char* filename);
	~Scaner() {}

};

