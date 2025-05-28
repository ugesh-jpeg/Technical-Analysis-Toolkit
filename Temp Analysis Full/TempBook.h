#pragma once
#include "TempBookEntry.h"
#include "Candlestick.h"
#include <string>
#include <vector>
#include <map>

class TempBook
{
public:
    TempBook(const std::string& filename); // Initialize with a CSV file
    void calculateCandlesticks();          // Calculate candlestick data
    void createTextBasedPlot();
    void displayCandlestickData();         // Display the calculated candlestick data
    void filterAndDisplayData();           // Filter data and display it
    void predictAndDisplayFutureData();    // Predict future data and display it

private:
    std::vector<TempBookEntry> entries;     // Vector to store temperature entries
    std::vector<Candlestick> candlesticks;  // Vector to store computed candlesticks
    std::map<std::string, std::vector<TempBookEntry>> groupByYear(); // Helper for aggregation
};
