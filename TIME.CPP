#include <string.h>
#include "time.h"

Time::Time()
{
    hours = 0;
    minutes = 0;
    seconds = 0;
}

Time::Time(int h, int m, int s)
{
    hours = h;
    minutes = m;
    seconds = s;
}

int Time::GetHours()const
{
    return hours;
}

void Time::SetHours(int h)
{
    hours = h;
}

int Time::GetMinutes()const
{
    return minutes;
}

void Time::SetMinutes(int m)
{
    minutes = m;
}

int Time::GetSeconds()const
{
    return seconds;
}

void Time::SetSeconds(int s)
{
    seconds = s;
}

std::string Time::GetDN() const
{
    return daynight;
}

void Time::SetDN(const std::string& dn)
{
    daynight = dn;
}

bool Time::operator<(const Time& other) const {
    if (hours != other.hours) return hours < other.hours;
    return minutes < other.minutes;
}

bool Time::operator>(const Time& other) const {
    if (hours != other.hours) return hours > other.hours;
    return minutes > other.minutes;
}

bool Time::operator==(const Time& other) const {
    return hours == other.hours && minutes == other.minutes;
}

std::ostream& operator<<(std::ostream& os, const Time& time) {
    os << time.GetHours() << ":" << time.GetMinutes();
    return os;
}

std::istream& operator>>(std::istream& is, Time& time) {
    int hours, minutes;
    char colon;
    is >> hours >> colon >> minutes;
    if (colon != ':') {
        is.setstate(std::ios::failbit);
    } else {
        time.SetHours(hours);
        time.SetMinutes(minutes);
    }
    return is;
}
