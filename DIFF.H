#ifndef DIFF_H
#define DIFF_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "Date.h"
#include "Time.h"
#include "VECTOR.H"
#include "BST.H"
#include <map>

typedef struct {
    Date d;
    Time t;
    float speed;
    float temperature;
    int solarRadiation;
} WindRecType;

typedef Vector<WindRecType> WindlogType;

extern BSTMap<Date, WindRecType> windLogMap;
extern BST<Date> dateBST;

std::istream& operator >> (std::istream& file, WindlogType& info);

void focusedBlocks(Vector<std::string> blocks, int& c1, int& c2, int& c3, int& c4);

void dateInput(std::string& date, Date& d);
void operator >> (std::stringstream& s, Date& d);

void timeInput(std::string& time, Time& t);
void operator >> (std::stringstream& s, Time& t);

void display(WindlogType& info);

double calculateAverage(Vector<float>& values);
double calculateStandardDeviation(Vector<float>& values, double mean);
double calculateTotalSolarRadiation(Vector<float>& solarRadiation);

Vector<float> getSpeed(WindlogType& selected);
Vector<float> getTemperature(WindlogType& selected);
Vector<float> getSolarRadiation(WindlogType& selected);

void userMonthYear(int& month, int& year);
void userYear(int& year);
void userMonth(int& month);
WindlogType selectedinfo(WindlogType& info, int month, int year);
WindlogType selectedinfoyear(WindlogType& info, int year);

void printAverageTempPerMonth(WindlogType& selectedyear, int year);
void printAvgSpeedTempSolarPerMonth(WindlogType& selectedyear, int year);

double sPCC(Vector<float>& x, Vector<float>& y);
double mad(Vector<float>& data);



#endif // DIFF_H
