#include "Date.h"

Date::Date() : day(0), month(0), year(0) {}

Date::Date(int da, int mo, int y) : day(da), month(mo), year(y) {}

int Date::GetDay() const {
    return day;
}

void Date::SetDay(int da) {
    day = da;
}

int Date::GetMonth() const {
    return month;
}

void Date::SetMonth(int mo) {
    month = mo;
}

int Date::GetYear() const {
    return year;
}

void Date::SetYear(int y) {
    year = y;
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator>(const Date& other) const {
    if (year != other.year) return year > other.year;
    if (month != other.month) return month > other.month;
    return day > other.day;
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}


std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.GetDay() << "/" << date.GetMonth() << "/" << date.GetYear();
    return os;
}

std::istream& operator>>(std::istream& is, Date& date) {
    int day, month, year;
    char slash1, slash2;
    is >> day >> slash1 >> month >> slash2 >> year;
    if (slash1 != '/' || slash2 != '/') {
        is.setstate(std::ios::failbit);
    } else {
        date.SetDay(day);
        date.SetMonth(month);
        date.SetYear(year);
    }
    return is;
}
