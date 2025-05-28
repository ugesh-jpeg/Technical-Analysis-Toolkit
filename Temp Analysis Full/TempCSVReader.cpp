#include "TempCSVReader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

TempCSVReader::TempCSVReader() {}

std::vector<TempBookEntry> TempCSVReader::readCSV(const std::string& csvFilename) {
    std::vector<TempBookEntry> entries;
    std::ifstream csvFile(csvFilename);
    std::string line;

    if (csvFile.is_open()) {
        while (std::getline(csvFile, line)) {
            try {
                auto tokens = tokenise(line, ',');
                TempBookEntry entry = stringsToTempBookEntry(tokens);
                entries.push_back(entry);
            } catch (const std::exception& e) {     
                continue;
            }
        }
        csvFile.close();
    } else {
        std::cerr << "Failed to open CSV file: " << csvFilename << std::endl;
    }

    std::cout << "TempCSVReader::readCSV read " << entries.size() << " valid entries" << std::endl;
    return entries;
}

std::vector<std::string> TempCSVReader::tokenise(const std::string& csvLine, char separator) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;

    while ((end = csvLine.find(separator, start)) != std::string::npos) {
        tokens.push_back(csvLine.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(csvLine.substr(start));
    return tokens;
}

TempBookEntry TempCSVReader::stringsToTempBookEntry(const std::vector<std::string>& tokens) {
    if (tokens.size() < 5) {
        throw std::runtime_error("Incorrect number of fields in CSV line");
    }

    try {
        std::string date = tokens[0];
        double open = std::stod(tokens[1]);
        double high = std::stod(tokens[2]);
        double low = std::stod(tokens[3]);
        double close = std::stod(tokens[4]);

        return TempBookEntry(date, open, high, low, close);
    } catch (const std::exception&) {
        throw std::runtime_error("Failed to parse tokens into TempBookEntry");
    }
}
