#pragma once
#include "TempBook.h"

class TempMain
{
public:
    TempMain();
    void init();

private:
    void printMenu();
    void printHelp();
    int getUserOption();
    void processUserOption(int userOption, TempBook& book);
};
