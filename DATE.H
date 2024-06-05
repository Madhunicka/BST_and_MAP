#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date {
public:
    Date();
    Date(int da, int mo, int y);

    int GetDay() const;
    void SetDay(int da);

    int GetMonth() const;
    void SetMonth(int mo);

    int GetYear() const;
    void SetYear(int y);


    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator==(const Date& other) const;

private:
    int day;
    int month;
    int year;
};
std::ostream& operator<<(std::ostream& os, const Date& date);
std::istream& operator>>(std::istream& is, Date& date);

#endif
