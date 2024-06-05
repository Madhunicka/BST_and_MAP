#ifndef TIME.H
#define TIME.H
#include <iostream>
#include <string.h>
#include <fstream>


class Time{

public:
    Time();
    Time(int h, int m, int s);

    int GetHours()const;
    void SetHours(int h);

    int GetMinutes()const;
    void SetMinutes(int m);

    int GetSeconds()const;
    void SetSeconds(int s);

    std::string GetDN()const;
    void SetDN(const std::string& dn);

    bool operator<(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator==(const Time& other) const;

private:
    int hours;
    int minutes;
    int seconds;
    std::string daynight;

};

std::ostream& operator<<(std::ostream& os, const Time& time);
std::istream& operator>>(std::istream& is, Time& time);

#endif // TIME
