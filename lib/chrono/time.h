#pragma once

#include <cstdint>

class Time
{
public:

    Time();

    double asHertz() const;

    double asHours() const;

    double asMinutes() const;

    double asSeconds() const;

    int64_t asMilliseconds() const;

    int64_t asMicroseconds() const;

    static Time now();

private:

    friend Time hertz(double);
    friend Time hours(double);
    friend Time minutes(double);
    friend Time seconds(double);
    friend Time milliseconds(int64_t);
    friend Time microseconds(int64_t);

    explicit Time(int64_t microseconds);

private:

    int64_t m_microseconds; ///< Time value stored as microseconds
};

void sleep(const Time& t);

Time hertz(double amount);

Time hours(double);

Time minutes(double);

Time seconds(double amount);

Time milliseconds(int64_t amount);

Time microseconds(int64_t amount);

bool operator ==(Time left, Time right);

bool operator !=(Time left, Time right);

bool operator <(Time left, Time right);

bool operator >(Time left, Time right);

bool operator <=(Time left, Time right);

bool operator >=(Time left, Time right);

Time operator -(Time right);

Time operator +(Time left, Time right);

Time& operator +=(Time& left, Time right);

Time operator -(Time left, Time right);

Time& operator -=(Time& left, Time right);

Time operator *(Time left, double right);

Time operator *(Time left, int64_t right);

Time operator *(double left, Time right);

Time operator *(int64_t left, Time right);

Time& operator *=(Time& left, double right);

Time& operator *=(Time& left, int64_t right);

Time operator /(Time left, double right);

Time operator /(Time left, int64_t right);

Time& operator /=(Time& left, double right);

Time& operator /=(Time& left, int64_t right);

double operator /(Time left, Time right);

Time operator %(Time left, Time right);

Time& operator %=(Time& left, Time right);