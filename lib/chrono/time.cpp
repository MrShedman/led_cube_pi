#include "time.h"

#include <chrono>
#include <thread>

Time Time::now()
{
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(now).count();

    return microseconds(us);
}

Time::Time() 
:
m_microseconds(0)
{
}

double Time::asHertz() const
{
    return 1.0 / asSeconds();
}

double Time::asHours() const
{
    return asMinutes() / 60.0;
}

double Time::asMinutes() const
{
    return asSeconds() / 60.0;
}

double Time::asSeconds() const
{
    return m_microseconds / 1000000.0;
}

int64_t Time::asMilliseconds() const
{
    return static_cast<int64_t>(m_microseconds / 1000);
}

int64_t Time::asMicroseconds() const
{
    return m_microseconds;
}

Time::Time(int64_t microseconds) :
m_microseconds(microseconds)
{
}

void sleep(const Time& t)
{
    std::this_thread::sleep_for(std::chrono::microseconds(t.asMicroseconds()));
}

Time hertz(double amount)
{
    return Time(static_cast<int64_t>((1.0/amount) * 1000000.0));
}

Time hours(double amount)
{
    return minutes(amount * 60.0);
}

Time minutes(double amount)
{
    return seconds(amount * 60.0);
}

Time seconds(double amount)
{
    return Time(static_cast<int64_t>(amount * 1000000.0));
}

Time milliseconds(int64_t amount)
{
    return Time(static_cast<int64_t>(amount) * 1000.0);
}

Time microseconds(int64_t amount)
{
    return Time(amount);
}

bool operator ==(Time left, Time right)
{
    return left.asMicroseconds() == right.asMicroseconds();
}

bool operator !=(Time left, Time right)
{
    return left.asMicroseconds() != right.asMicroseconds();
}

bool operator <(Time left, Time right)
{
    return left.asMicroseconds() < right.asMicroseconds();
}

bool operator >(Time left, Time right)
{
    return left.asMicroseconds() > right.asMicroseconds();
}

bool operator <=(Time left, Time right)
{
    return left.asMicroseconds() <= right.asMicroseconds();
}

bool operator >=(Time left, Time right)
{
    return left.asMicroseconds() >= right.asMicroseconds();
}

Time operator -(Time right)
{
    return microseconds(-right.asMicroseconds());
}

Time operator +(Time left, Time right)
{
    return microseconds(left.asMicroseconds() + right.asMicroseconds());
}

Time& operator +=(Time& left, Time right)
{
    return left = left + right;
}

Time operator -(Time left, Time right)
{
    return microseconds(left.asMicroseconds() - right.asMicroseconds());
}

Time& operator -=(Time& left, Time right)
{
    return left = left - right;
}

Time operator *(Time left, double right)
{
    return seconds(left.asSeconds() * right);
}

Time operator *(Time left, int64_t right)
{
    return microseconds(left.asMicroseconds() * right);
}

Time operator *(double left, Time right)
{
    return right * left;
}

Time operator *(int64_t left, Time right)
{
    return right * left;
}

Time& operator *=(Time& left, double right)
{
    return left = left * right;
}

Time& operator *=(Time& left, int64_t right)
{
    return left = left * right;
}

Time operator /(Time left, double right)
{
    return seconds(left.asSeconds() / right);
}

Time operator /(Time left, int64_t right)
{
    return microseconds(left.asMicroseconds() / right);
}

Time& operator /=(Time& left, double right)
{
    return left = left / right;
}

Time& operator /=(Time& left, int64_t right)
{
    return left = left / right;
}

double operator /(Time left, Time right)
{
    return left.asSeconds() / right.asSeconds();
}

Time operator %(Time left, Time right)
{
    return microseconds(left.asMicroseconds() % right.asMicroseconds());
}

Time& operator %=(Time& left, Time right)
{
    return left = left % right;
}