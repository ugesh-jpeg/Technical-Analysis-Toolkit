#include "TempMain.h"
#include <iostream>

TempMain::TempMain() {}

void TempMain::init()
{
    TempBook book("weather_data_EU_1980-2019_temp_only.csv");
    book.calculateCandlesticks();

    int input;
    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input, book);
    }
}

void TempMain::printMenu()
{
    std::cout << std::endl;
    std::cout << "Welcome to ThA TV WD\n";
    std::cout << "Technical analysis toolkit for visualising and predicting weather data\n";
    std::cout << "1: Compute Candlestick Data\n";
    std::cout << "2: Text-based plot of Candlestick Data\n";
    std::cout << "3: Filter and Display Data\n";
    std::cout << "4: Predict and Display Future Data\n";
    std::cout << "5: Help\n";
    std::cout << std::endl;
    std::cout << "Enter option (1-5): ";
}

int TempMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        std::cout << "Invalid input. Please enter a number 1-5." << std::endl;
    }
    return userOption;
}

void TempMain::processUserOption(int userOption, TempBook &book)
{
    switch (userOption)
    {
    case 1:
        book.displayCandlestickData();
        break;
    case 2:
        book.createTextBasedPlot();
        break;
    case 3:
        book.filterAndDisplayData();
        break;
    case 4:
        book.predictAndDisplayFutureData();
        break;
    case 5:
        printHelp();
        break;
    default:
        std::cout << "Invalid choice. Choose 1-5." << std::endl;
        break;
    }
}

void TempMain::printHelp()
{
    std::cout << std::endl;
    std::cout << "Help - Navigate through the options to visualize and analyze temperature data:\n";
    std::cout << "1 - Displays computed candlestick charts for temperature data.\n";
    std::cout << "2 - Displays text-based plot of the candlestick data.\n";
    std::cout << "3 - Filters data based on user-specified criteria and displays it.\n";
    std::cout << "4 - Uses historical data to predict future temperature trends.\n";
}