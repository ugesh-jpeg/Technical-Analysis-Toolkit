#include "Candlestick.h"

Candlestick::Candlestick(const std::string& _timeframe, double _open, double _high, double _low, double _close)
    : timeframe(_timeframe), open(_open), high(_high), low(_low), close(_close) {}
