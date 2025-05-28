#include "TempBook.h"
#include "TempCSVReader.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <limits>
#include <set>
#include <random>

TempBook::TempBook(const std::string &filename)
{
    entries = TempCSVReader::readCSV(filename);
}

void TempBook::calculateCandlesticks()
{
    std::map<std::string, std::vector<TempBookEntry>> groupedData = groupByYear();

    std::vector<TempBookEntry> previousYearEntries;

    // Interpolation for first year's "open" value
    bool firstYearProcessed = false;

    for (std::map<std::string, std::vector<TempBookEntry>>::iterator it = groupedData.begin(); it != groupedData.end(); ++it)
    {
        const std::string &year = it->first;
        std::vector<TempBookEntry> &yearlyEntries = it->second;

        // Ensure entries are sorted by date
        std::sort(yearlyEntries.begin(), yearlyEntries.end(),
                  [](const TempBookEntry &a, const TempBookEntry &b)
                  {
                      return a.date < b.date;
                  });

        double open, close, high, low;

        if (!firstYearProcessed)
        {
            // Calculate the "open" value for 1980 as the median of open and close
            open = ((*std::max_element(yearlyEntries.begin(), yearlyEntries.end(),
                                       [](const TempBookEntry &a, const TempBookEntry &b)
                                       {
                                           return a.high < b.high;
                                       }))
                        .high +
                    (*std::min_element(yearlyEntries.begin(), yearlyEntries.end(),
                                       [](const TempBookEntry &a, const TempBookEntry &b)
                                       {
                                           return a.low < b.low;
                                       }))
                        .low) /
                   2.0;
            firstYearProcessed = true;
        }
        else
        {
            if (previousYearEntries.empty()) {
                open = (yearlyEntries[0].high + yearlyEntries[0].low) / 2.0;
            } else {
                double sum = 0.0;
                for (const TempBookEntry &entry : previousYearEntries) {
                    sum += entry.open;
                }
                open = sum / previousYearEntries.size();
            }
}


        // Compute Close: Average of mean temperatures for the current year
        close = std::accumulate(yearlyEntries.begin(), yearlyEntries.end(), 0.0,
                                [](double sum, const TempBookEntry &entry)
                                {
                                    return sum + entry.open;
                                }) /
                yearlyEntries.size();

        // Compute High: Maximum temperature in the current year
        high = (*std::max_element(yearlyEntries.begin(), yearlyEntries.end(),
                                  [](const TempBookEntry &a, const TempBookEntry &b)
                                  {
                                      return a.high < b.high;
                                  }))
                   .high;

        // Compute Low: Minimum temperature in the current year
        low = (*std::min_element(yearlyEntries.begin(), yearlyEntries.end(),
                                 [](const TempBookEntry &a, const TempBookEntry &b)
                                 {
                                     return a.low < b.low;
                                 }))
                  .low;

        // Add the computed candlestick to the vector
        candlesticks.emplace_back(year, open, high, low, close);

        // Update previous year data
        previousYearEntries = yearlyEntries;
    }
}

void TempBook::displayCandlestickData()
{
    std::cout << std::endl;
    std::cout << "Yearly Candlestick Data:\n";
    std::cout << "Year     | Open     | High     | Low      | Close    " << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;

    for (std::vector<Candlestick>::const_iterator it = candlesticks.begin(); it != candlesticks.end(); ++it)
    {
        const Candlestick &candle = *it;
        std::cout << std::setw(8) << candle.timeframe
                  << " | " << std::setw(8) << std::fixed << std::setprecision(3) << candle.open
                  << " | " << std::setw(8) << candle.high
                  << " | " << std::setw(8) << candle.low
                  << " | " << std::setw(8) << candle.close << std::endl;
    }
    std::cout << std::endl;
}

void TempBook::createTextBasedPlot()
{
    if (candlesticks.empty())
    {
        std::cout << "No candlestick data available to plot." << std::endl;
        return;
    }

    const int plotHeight = 10;

    std::set<std::string> selectedYears;
    std::cout << "Enter up to 5 years to plot (separated by space): ";
    std::string year;
    for (int i = 0; i < 5; ++i)
    {
        std::cin >> year;
        selectedYears.insert(year);
        if (std::cin.peek() == '\n')
            break;
    }
    std::cout << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Filter candlestick data based on selected years
    std::vector<Candlestick> filteredCandlesticks;
    for (std::vector<Candlestick>::iterator it = candlesticks.begin(); it != candlesticks.end(); ++it)
    {
        if (selectedYears.count(it->timeframe))
        {
            filteredCandlesticks.push_back(*it);
        }
    }

    if (filteredCandlesticks.empty())
    {
        std::cout << "No data available for the specified years." << std::endl;
        return;
    }

    // Determine the scaling range
    double maxVal = filteredCandlesticks[0].high;
    double minVal = filteredCandlesticks[0].low;

    // Calculate the maximum and minimum values across all selected years
    for (std::vector<Candlestick>::iterator it = filteredCandlesticks.begin(); it != filteredCandlesticks.end(); ++it)
    {
        if (it->high > maxVal)
            maxVal = it->high;
        if (it->low < minVal)
            minVal = it->low;
    }

    // Ensure the range is not too small for scaling
    double range = maxVal - minVal;
    if (range < 1.0)
    {
        range = 1.0;
    }

    double scale = range / plotHeight;

    // Generate the plot row by row
    std::vector<std::string> rows(plotHeight, std::string(filteredCandlesticks.size() * 10, ' '));

    for (size_t i = 0; i < filteredCandlesticks.size(); ++i)
    {
        Candlestick candle = filteredCandlesticks[i];

        // Calculate the vertical positions for open and close
        int openPos = std::max(0, std::min(plotHeight - 1, static_cast<int>(plotHeight - std::round((candle.open - minVal) / scale) - 1)));
        int closePos = std::max(0, std::min(plotHeight - 1, static_cast<int>(plotHeight - std::round((candle.close - minVal) / scale) - 1)));

        // Draw high-low range (vertical line)
        for (int j = 1; j <= plotHeight; ++j)
        {
            rows[j][i * 10 + 5] = '|'; // Represent high-low range with vertical line
        }

        // Draw open and close
        if (openPos < closePos)
        {
            // Draw the candlestick body from open to close
            for (int j = openPos + 1; j < closePos; ++j)
            {
                rows[j][i * 10 + 4] = '-';
                rows[j][i * 10 + 5] = '-';
                rows[j][i * 10 + 6] = '-';
            }

            // Draw open and close as '+' symbols
            rows[openPos][i * 10 + 4] = '+';
            rows[openPos][i * 10 + 5] = '+';
            rows[openPos][i * 10 + 6] = '+';

            rows[closePos][i * 10 + 4] = '+';
            rows[closePos][i * 10 + 5] = '+';
            rows[closePos][i * 10 + 6] = '+';
        }
        else
        {
            // Inverted candlestick body if close is less than open
            for (int j = closePos + 1; j < openPos; ++j)
            {
                rows[j][i * 10 + 4] = '-';
                rows[j][i * 10 + 5] = '-';
                rows[j][i * 10 + 6] = '-';
            }

            // Draw inverted open and close as '+' symbols
            rows[closePos][i * 10 + 4] = '+';
            rows[closePos][i * 10 + 5] = '+';
            rows[closePos][i * 10 + 6] = '+';

            rows[openPos][i * 10 + 4] = '+';
            rows[openPos][i * 10 + 5] = '+';
            rows[openPos][i * 10 + 6] = '+';
        }
    }

    // Print the plot row by row
    for (int i = 0; i < plotHeight; ++i)
    {
        std::cout << std::setw(3) << plotHeight - i << " |" << rows[i] << std::endl;
    }

    // Print the horizontal axis
    std::cout << "    " << std::string(filteredCandlesticks.size() * 10, '-') << std::endl;

    // Print the years at the bottom
    std::cout << "     ";
    for (size_t i = 0; i < filteredCandlesticks.size(); ++i)
    {
        std::cout << std::setw(10) << filteredCandlesticks[i].timeframe;
    }
    std::cout << std::endl;
}

void TempBook::filterAndDisplayData()
{
    std::string year;
    std::cout << "Enter the year to filter by: ";
    std::cin >> year;
    std::cout << std::endl;

    // Avoid affecting subsequent input error
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Find and display candlestick data for the specified year
    bool found = false;
    for (const Candlestick &candle : candlesticks)
    {
        if (candle.timeframe == year)
        {
            std::cout << "Year     | Open     | High     | Low      | Close    " << std::endl;
            std::cout << "------------------------------------------------------" << std::endl;
            std::cout << std::setw(8) << candle.timeframe
                      << " | " << std::setw(8) << std::fixed << std::setprecision(3) << candle.open
                      << " | " << std::setw(8) << candle.high
                      << " | " << std::setw(8) << candle.low
                      << " | " << std::setw(8) << candle.close << std::endl;
            found = true;
            break;
        }
    }

    if (!found)
    {
        std::cout << "No data found for the year: " << year << std::endl;
    }
    std::cout << std::endl;
}

void TempBook::predictAndDisplayFutureData()
{
    if (candlesticks.empty())
    {
        std::cout << "No data available for prediction." << std::endl;
        return;
    }

    const Candlestick &lastCandle = candlesticks.back();

    // Predict the next year's candlestick data
    std::string nextYear = std::to_string(std::stoi(lastCandle.timeframe) + 1);

    // Generate random values between -2.999 and 2.999
    double randomVariationHigh = (std::rand() % 6000 - 3000) / 1000.0;
    double randomVariationLow = (std::rand() % 6000 - 3000) / 1000.0; 

    double predictedOpen = lastCandle.close;
    double predictedHigh = lastCandle.high + randomVariationHigh;
    double predictedLow = lastCandle.low + randomVariationLow;
    double predictedClose = (predictedHigh + predictedLow) / 2.0; // Average of high and low

    // Display the predicted candlestick
    std::cout << std::endl;
    std::cout << "Predicted Candlestick Data for " << nextYear << ":\n";
    std::cout << "Year     | Open     | High     | Low      | Close    " << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << std::setw(8) << nextYear
              << " | " << std::setw(8) << std::fixed << std::setprecision(3) << predictedOpen
              << " | " << std::setw(8) << predictedHigh
              << " | " << std::setw(8) << predictedLow
              << " | " << std::setw(8) << predictedClose << std::endl;
    std::cout << std::endl;
}

std::map<std::string, std::vector<TempBookEntry>> TempBook::groupByYear()
{
    std::map<std::string, std::vector<TempBookEntry>> groupedData;

    for (std::vector<TempBookEntry>::iterator it = entries.begin(); it != entries.end(); ++it)
    {
        std::string year = it->date.substr(0, 4);
        groupedData[year].push_back(*it);
    }

    return groupedData;
}