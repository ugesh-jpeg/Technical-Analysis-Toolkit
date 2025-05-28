#pragma once

#include "TempBookEntry.h"
#include <vector>
#include <string>

class TempCSVReader {
public:
    TempCSVReader();

    static std::vector<TempBookEntry> readCSV(const std::string& csvFile);
    static std::vector<std::string> tokenise(const std::string& csvLine, char separator);

private:
    static TempBookEntry stringsToTempBookEntry(const std::vector<std::string>& tokens);
};
