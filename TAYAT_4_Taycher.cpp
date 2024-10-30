#include <iostream>
#include <iomanip>
#include "Scaner.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL,"rus");
    Scaner* scaner;
    int type;
    Type_lex lex;

    if (argc <= 1)
        scaner = new Scaner("input.txt");
    else
        scaner = new Scaner(argv[1]);
    do
    {
        type = scaner->UseScaner(lex);
        std::cout << std::setw(32) << lex << "      принадлежит к типу     " << type << std::endl;
    } while (type != TEnd);

    delete scaner;
    return 0;
}
