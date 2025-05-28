#pragma once
#include <string>

class TempBookEntry
{
public:
    TempBookEntry(const std::string& _date, double _open, double _high, double _low, double _close);

    std::string date;   // Date of the data entry
    double open;        // Opening temperature
    double high;        // Highest temperature
    double low;         // Lowest temperature
    double close;       // Closing temperature
};