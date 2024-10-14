#pragma once
#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <ctime>
#include <cstdint>

struct DateTime
{
    DateTime() : year(0), month(0), day(0), hour(0), minute(0), second(0)  {}

    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
    uint16_t second;

    static inline DateTime getCurrentDateTime()
    {
        const std::time_t now = std::time(nullptr);
        const std::tm *const local_time = localtime(&now);

        DateTime dt;
        dt.year = 1900 + local_time->tm_year;
        dt.month = 1 + local_time->tm_mon;
        dt.day = local_time->tm_mday;
        dt.hour = local_time->tm_hour;
        dt.minute = local_time->tm_min;
        dt.second = local_time->tm_sec;
        return dt;
    }
};

#endif // !TIME_UTILS_H
