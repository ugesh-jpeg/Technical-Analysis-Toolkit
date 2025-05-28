#pragma once
#include <string>

class Candlestick
{
public:
    Candlestick(const std::string& year, double open, double high, double low, double close);

    std::string timeframe;
    double open;
    double high;
    double low;
    double close;
};
