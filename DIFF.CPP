#include <iostream>
#include "diff.h"
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>


static const std::string monthNames[] = {
    "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

std::multimap<Date, WindRecType> windLogMap;
BST<Date> dateBST;

std::istream & operator >> (std::istream& file, WindlogType& info)
{
    Date d = Date();
    Time t = Time();
    float speed = 0.0;
    float temperature = 0.0;
    int solarRadiation = 0;

    int c1, c2, c3, c4;
    std::string temp, block;

    Vector <std::string> blocks;

    getline(file, temp);
    std::stringstream s1(temp);
    while (std::getline(s1, block, ',')) {
        blocks.push_back(block);
    }

    focusedBlocks(blocks, c1, c2, c3, c4);
    blocks.clear();

    std::string temp2;
    int i = 0;
    while (getline(file, temp2)) {
        std::stringstream s2(temp2);
        while (std::getline(s2, block, ',')) {
            blocks.push_back(block);
        }

        std::string datetimeStr = (blocks[c1]);
        if (!datetimeStr.empty() && datetimeStr != "N/A") {
            std::stringstream datetime(datetimeStr);
            std::string date, time;

            getline(datetime, date, ' ');
            dateInput(date, d);

            getline(datetime, time);
            timeInput(time, t);
        }

        speed = (blocks[c2].empty() || blocks[c2] == "N/A" || blocks[c2].find_first_not_of(" \t\r\n") == std::string::npos) ? 0.0 : std::stof(blocks[c2]);
        temperature = (blocks[c3].empty() || blocks[c3] == "N/A" || blocks[c3].find_first_not_of(" \t\r\n") == std::string::npos) ? 0.0 : std::stof(blocks[c3]);
        solarRadiation = (blocks[c4].empty() || blocks[c4] == "N/A" || blocks[c4].find_first_not_of(" \t\r\n") == std::string::npos) ? 0 : std::stoi(blocks[c4]);

        WindRecType w = { d, t, speed, temperature, solarRadiation };
        info.push_back(w);
        windLogMap.insert({w.d, w});
        dateBST.insert(w.d);



        blocks.clear();
        i++;
    }
    return file;
}

void focusedBlocks(Vector<std::string> blocks, int& c1, int& c2, int& c3, int& c4)
{
    for (int i = 0; i < blocks.getSize(); i++) {
        if (blocks[i] == "WAST") {
            c1 = i;
        }
        else if (blocks[i] == "S") {
            c2 = i;
        }
        else if (blocks[i] == "T") {
            c3 = i;
        }
        else if (blocks[i] == "SR") {
            c4 = i;
        }
    }
}

void dateInput(std::string& date, Date& d)
{
    std::stringstream stream(date);
    stream >> d;
}

void operator >> (std::stringstream& s, Date& d)
{
    std::string temp;
    getline(s, temp, '/');
    d.SetDay(std::stoi(temp));
    getline(s, temp, '/');
    d.SetMonth(std::stoi(temp));
    getline(s, temp);
    d.SetYear(std::stoi(temp));
}

void timeInput(std::string& time, Time& t)
{
    std::stringstream stream(time);
    stream >> t;
}

void operator >> (std::stringstream& s, Time& t)
{
    std::string temp;
    getline(s, temp, ':');
    t.SetHours(std::stoi(temp));
    getline(s, temp, ':');
    t.SetMinutes(std::stoi(temp));
}

void display(WindlogType& info)
{
    std::cout << "Press 1 to get Avg Wind Speed and Standard Deviation by month & year: " << std::endl;
    std::cout << "Press 2 to get Avg Temperature and Standard Deviation of Temperature by year: " << std::endl;
    std::cout << "Press 3 to get Sample Pearson Correlation Coefficient by Month for the specified year (S_T,S_R,T_R): " << std::endl;
    std::cout << "Press 4 to get Avg Wind Speed, Avg Air Temp & Total Solar Radiation by year: " << std::endl;
    std::cout << "\n" << std::endl;

    while (true)
    {
        int choice = -1;
        std::cin >> choice;
        switch (choice)
        {
        case 1:
        {
            int month, year;
            userMonthYear(month, year);
            WindlogType selected = selectedinfo(info, month, year);
            if (selected.getSize() != 0)
            {
                Vector<float> speed = getSpeed(selected);
                double mean = calculateAverage(speed) * 3.6;
                double stdDev = calculateStandardDeviation(speed, mean);
                std::cout << "\n" << std::endl;
                std::cout << "Average Wind Speed for Month: " << monthNames[month - 1] << " and year " << year << ": " << mean << std::endl;
                std::cout << "Standard Deviation of Wind Speed for month: " << monthNames[month - 1] << " and year " << year << ": " << stdDev << std::endl;
            }
            else
                std::cout << monthNames[month - 1] << "No Data!!!";
            break;
        }
        case 2:
        {
            int year;
            userYear(year);
            WindlogType selectedyear = selectedinfoyear(info, year);
            printAverageTempPerMonth(selectedyear, year);
            break;
        }
        case 3:
        {
            int month;
            userMonth(month);
            for (int year = info[0].d.GetYear(); year <= info[info.getSize() - 1].d.GetYear(); ++year)
            {
                WindlogType selected;
                selected = selectedinfo(info, month, year);
                if (selected.getSize() > 1)
                {
                    Vector<float> speed = getSpeed(selected);
                    Vector<float> temperature = getTemperature(selected);
                    Vector<float> solarRadiation = getSolarRadiation(selected);

                    double spcc_speed_temp = sPCC(speed, temperature);
                    double spcc_speed_solar = sPCC(speed, solarRadiation);
                    double spcc_temp_solar = sPCC(temperature, solarRadiation);
                    std::cout << "sPCC for Speed and Temperature for " << monthNames[month - 1] << " " << year << ": " << spcc_speed_temp << std::endl;
                    std::cout << "sPCC for Speed and Solar Radiation for " << monthNames[month - 1] << " " << year << ": " << spcc_speed_solar << std::endl;
                    std::cout << "sPCC for Temperature and Solar Radiation for " << monthNames[month - 1] << " " << year << ": " << spcc_temp_solar << std::endl;
                }
                else
                {
                    std::cout << "Insufficient data for month " << monthNames[month - 1] << " and year " << year << std::endl;
                }
            }
            break;
        }
        case 4:
        {
            int year;
            userYear(year);
            WindlogType selectedyear = selectedinfoyear(info, year);
            printAvgSpeedTempSolarPerMonth(info, year);
            break;
        }
        default:
        {
            std::cout << "Invalid choice. Please choose again." << std::endl;
        }
        }
        std::cout << "\n" << std::endl;
        std::cout << "Press 1 to get Avg Wind Speed and Standard Deviation by month & year: " << std::endl;
        std::cout << "Press 2 to get Avg Temperature and Standard Deviation of Temperature by year: " << std::endl;
        std::cout << "Press 3 to get Sample Pearson Correlation Coefficient by Month for the specified year (S_T,S_R,T_R): " << std::endl;
        std::cout << "Press 4 to get Avg Wind Speed, Avg Air Temp & Total Solar Radiation by year: " << std::endl;
        std::cout << "\n" << std::endl;
    }
}

void userMonthYear(int& month, int& year)
{
    std::cout << "Enter the Month from 1-12 in Number: ";
    std::cin >> month;
    while (month < 1 || month > 12)
    {
        std::cout << "Invalid Input! Try Again: ";
        std::cin >> month;
    }

    std::cout << "Enter the Year (eg: 2020): ";
    std::cin >> year;
}

void userMonth(int& month)
{
    std::cout << "Enter the Month from 1-12 in Number: ";
    std::cin >> month;
    while (month < 1 || month > 12)
    {
        std::cout << "Invalid Input! Try Again: ";
        std::cin >> month;
    }
}

void userYear(int& year)
{
    std::cout << "Enter the Year (eg: 2020): ";
    std::cin >> year;
}

WindlogType selectedinfo(WindlogType& info, int month, int year)
{
    WindlogType selected;
    for (int i = 0; i < info.getSize(); i++)
    {
        if (info[i].d.GetMonth() == month && info[i].d.GetYear() == year)
        {
            selected.push_back(info[i]);
        }
    }
    return selected;
}

WindlogType selectedinfoyear(WindlogType& info, int year)
{
    WindlogType selectedyear;
    for (int i = 0; i < info.getSize(); i++)
    {
        if (info[i].d.GetYear() == year)
        {
            selectedyear.push_back(info[i]);
        }
    }
    return selectedyear;
}

Vector<float> getSpeed(WindlogType& selected)
{
    Vector<float> speed;
    for (int i = 0; i < selected.getSize(); i++)
    {
        speed.push_back(selected[i].speed);
    }
    return speed;
}

Vector<float> getTemperature(WindlogType& selected)
{
    Vector<float> temperature;
    for (int i = 0; i < selected.getSize(); i++)
    {
        temperature.push_back(selected[i].temperature);
    }
    return temperature;
}

Vector<float> getSolarRadiation(WindlogType& selected)
{
    Vector<float> solarRadiation;
    for (int i = 0; i < selected.getSize(); i++)
    {
        solarRadiation.push_back(selected[i].solarRadiation);
    }
    return solarRadiation;
}

double calculateAverage(Vector<float>& values)
{
    double sum = 0.0;
    for (int i = 0; i < values.getSize(); i++)
    {
        sum += values[i];
    }
    return sum / values.getSize();
}

double calculateStandardDeviation(Vector<float>& values, double mean)
{
    double sum = 0.0;
    for (int i = 0; i < values.getSize(); i++)
    {
        sum += pow(values[i] - mean, 2);
    }
    return sqrt(sum / values.getSize());
}

void printAverageTempPerMonth(WindlogType& selectedyear, int year)
{
    for (int month = 1; month <= 12; month++)
    {
        WindlogType selected = selectedinfo(selectedyear, month, year);
        if (selected.getSize() != 0)
        {
            Vector<float> temperature = getTemperature(selected);
            double mean = calculateAverage(temperature);
            double stdDev = calculateStandardDeviation(temperature, mean);
            std::cout << "\n" << std::endl;
            std::cout << "Average Temperature for Month: " << monthNames[month - 1] << " and year " << year << ": " << mean << std::endl;
            std::cout << "Standard Deviation of Temperature for month: " << monthNames[month - 1] << " and year " << year << ": " << stdDev << std::endl;
        }
        else
            std::cout << monthNames[month - 1] << "           No Data!!!\n";
    }
}

void printAvgSpeedTempSolarPerMonth(WindlogType& info, int year) {
    std::ofstream outputFile("WindTempSolar.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }

    outputFile << "Month,Avg Wind Speed (Std Dev, Mad),Avg Air Temp (Std Dev, Mad),Total Solar Radiation\n";

    for (int month = 1; month <= 12; ++month) {
        WindlogType selectedMonth = selectedinfo(info, month, year);
        if (selectedMonth.getSize() != 0) {
            Vector<float> speed = getSpeed(selectedMonth);
            Vector<float> temperature = getTemperature(selectedMonth);
            Vector<float> solarRadiation = getSolarRadiation(selectedMonth);

            double avgSpeed = (speed.getSize() == 0) ? NAN : calculateAverage(speed) * 3.6;
            double stdDevSpeed = (speed.getSize() == 0) ? NAN : calculateStandardDeviation(speed, avgSpeed);
            double madSpeed = (speed.getSize() == 0) ? NAN : mad(speed);

            double avgTemp = (temperature.getSize() == 0) ? NAN : calculateAverage(temperature);
            double stdDevTemp = (temperature.getSize() == 0) ? NAN : calculateStandardDeviation(temperature, avgTemp);
            double madTemp = (temperature.getSize() == 0) ? NAN : mad(temperature);

            double solarR = (solarRadiation.getSize() == 0) ? NAN : calculateTotalSolarRadiation(solarRadiation);

            outputFile << monthNames[month - 1] << ",";
            if (!std::isnan(avgSpeed)) {
                outputFile << avgSpeed << " (" << stdDevSpeed << ", " << madSpeed << "),";
            } else {
                outputFile << ",";
            }

            if (!std::isnan(avgTemp)) {
                outputFile << avgTemp << " (" << stdDevTemp << ", " << madTemp << "),";
            } else {
                outputFile << ",";
            }

            if (!std::isnan(solarR)) {
                outputFile << solarR;
            }

            outputFile << std::endl;
        }
    }

    outputFile.close();
}

double calculateTotalSolarRadiation(Vector<float>& solarRadiation)
{
    double sum = 0.0;
    for (int i = 0; i < solarRadiation.getSize(); i++)
    {
        if(solarRadiation[i]>100)
            {
                sum += solarRadiation[i];
            }
    }
    int temp = (sum/6)/100;
    return temp;
}

double sPCC(Vector<float>& x, Vector<float>& y)
{
    int n = x.getSize();
    double mean_x = calculateAverage(x);
    double mean_y = calculateAverage(y);

    double sum_xy = 0.0;
    double sum_x2 = 0.0;
    double sum_y2 = 0.0;

    for (int i = 0; i < n; i++)
    {
        sum_xy += (x[i] - mean_x) * (y[i] - mean_y);
        sum_x2 += (x[i] - mean_x) * (x[i] - mean_x);
        sum_y2 += (y[i] - mean_y) * (y[i] - mean_y);
    }

    return sum_xy / sqrt(sum_x2 * sum_y2);
}


double mad(Vector<float>& data) {
    if(data.getSize() == 0) {
        std::cerr << "Error: Vector data cannot be empty." << std::endl;
        return NAN;
    }

    double mean = calculateAverage(data);
    double sum_deviation = 0.0;
    for (size_t i = 0; i < data.getSize(); ++i) {
        sum_deviation += std::abs(data[i] - mean);
    }

    return sum_deviation / data.getSize();
}



