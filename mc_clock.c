/**
 * @file mc_clock.c
 */

#include "mc_clock.h"
#include <stdlib.h>

// 01/jan/2020 12:00:00 AM
#define DEFAULT_TIMESTAMP ((int32_t)1577836800)

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} clock_datetime_t;

typedef struct
{
    int32_t timestamp;
    clock_datetime_t datetime;
} clock_t;


// ##############################  PRIVATE FUNCTIONS  ################################# //

static uint8_t is_leap_year(uint16_t year)
{
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}// end is_leap_year

static uint8_t days_in_month(uint8_t month, uint16_t year)
{
    // dim: days in month
    static const uint8_t dim[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // leap year: february has 29 days
    if (month == 2 && is_leap_year(year))
        return 29;

    // return days in month
    return dim[month - 1];
}// end days_in_month

static clock_datetime_t Mc_Clock_Timestamp_To_Human_Date(int32_t timestamp)
{
    clock_datetime_t t;
    int64_t days = timestamp / 86400LL;
    int64_t seconds = timestamp % 86400LL;

    if (seconds < 0)
    {
        seconds += 86400LL;
        days -= 1;
    }

    t.hour = (uint8_t)(seconds / 3600);
    seconds %= 3600;
    t.minute = (uint8_t)(seconds / 60);
    t.second = (uint8_t)(seconds % 60);

    int32_t year = 1970;

    if (days >= 0)
    {
        while (1)
        {
            int32_t dim = is_leap_year(year) ? 366 : 365;
            if (days < dim)
                break;
            days -= dim;
            year++;
        }
    }
    else
    {
        while (1)
        {
            int32_t prev_year = year - 1;
            int32_t dim = is_leap_year(prev_year) ? 366 : 365;
            days += dim;
            if (days >= 0)
            {
                year = prev_year;
                break;
            }
            year = prev_year;
        }
    }

    t.year = (uint16_t)year;

    t.month = 1;
    while (1)
    {
        uint8_t dim = days_in_month(t.month, t.year);
        if (days < dim)
            break;
        days -= dim;
        t.month++;
    }

    t.day = (uint8_t)(days + 1);

    return t;
}// end Mc_Clock_Timestamp_To_Human_Date

static int32_t Mc_Clock_Human_Date_To_Timestamp(const clock_datetime_t *t)
{
    int64_t days = 0;
    int32_t year = t->year;

    if (year >= 1970)
    {
        for (int y = 1970; y < year; y++)
            days += is_leap_year(y) ? 366 : 365;
    }
    else
    {
        for (int y = 1969; y >= year; y--)
            days -= is_leap_year(y) ? 366 : 365;
    }

    for (uint8_t m = 1; m < t->month; m++)
        days += days_in_month(m, t->year);

    days += (t->day - 1);

    int64_t timestamp = days * 86400LL + (int64_t)t->hour * 3600 + (int64_t)t->minute * 60 + t->second;

    if (timestamp > INT32_MAX)
        timestamp = INT32_MAX;
    else if (timestamp < INT32_MIN)
        timestamp = INT32_MIN;

    return (int32_t)timestamp;
}// end Mc_Clock_Human_Date_To_Timestamp




// ##############################  PUBLIC FUNCTIONS  ################################# //




// ==================   Object Manipulation   ================ //

void *Mc_Clock_New(void)
{
    clock_t *p = malloc(sizeof(clock_t));
    p->timestamp = DEFAULT_TIMESTAMP;
    p->datetime = Mc_Clock_Timestamp_To_Human_Date(DEFAULT_TIMESTAMP);
    return p;
}// end Mc_Clock_New

void *Mc_Clock_Clone(void *clock)
{
    clock_t *_clock = clock;
    clock_t *p = Mc_Clock_New();

    p->datetime = _clock->datetime;
    p->timestamp = _clock->timestamp;

    return (void *)p;
}// end Mc_Clock_Clone

void Mc_Clock_Destroy(void *clock)
{
    free((clock_t *)clock);
}// end Mc_Clock_Destroy




// ==================   Clearers   ================ //

void Mc_Clock_Clear_Time(void *clock)
{
    clock_t *_clock = clock;

    _clock->datetime.hour = 0;
    _clock->datetime.minute = 0;
    _clock->datetime.second = 0;

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Clear_Time

void Mc_Clock_Clear_DateTime(void *clock)
{
    clock_t *_clock = clock;

    _clock->timestamp = DEFAULT_TIMESTAMP;

    // update datetime
    _clock->datetime = Mc_Clock_Timestamp_To_Human_Date(_clock->timestamp);
}// end Mc_Clock_Clear_DateTime




// ==================   Setters   ================ //

void Mc_Clock_Set_Timestamp(void *clock, int32_t timestamp)
{
    clock_t *_clock = clock;
    _clock->timestamp = timestamp;
    _clock->datetime = Mc_Clock_Timestamp_To_Human_Date(timestamp);
}// end Mc_Clock_Set_Timestamp

void Mc_Clock_Set_Second(void *clock, uint8_t second)
{
    if (second > 59)
        return;

    clock_t *_clock = clock;
    _clock->datetime.second = second;
    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Set_Second

void Mc_Clock_Set_Minute(void *clock, uint8_t minute)
{
    if (minute > 59)
        return;

    clock_t *_clock = clock;
    _clock->datetime.minute = minute;
    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Set_Minute

void Mc_Clock_Set_Hour(void *clock, uint8_t hour)
{
    if (hour > 23)
        return;

    clock_t *_clock = clock;
    _clock->datetime.hour = hour;
    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Set_Hour

void Mc_Clock_Set_Time(void *clock, uint8_t hour, uint8_t minute, uint8_t second)
{
    Mc_Clock_Set_Hour(clock, hour);
    Mc_Clock_Set_Minute(clock, minute);
    Mc_Clock_Set_Second(clock, second);
}// end Mc_Clock_Set_Time

void Mc_Clock_Set_Day(void *clock, uint8_t day)
{
    if (day == 0 || day > 31)
        return;

    clock_t *_clock = clock;

    // verify if day is in month
    uint8_t dim = days_in_month(_clock->datetime.month, _clock->datetime.year);
    if (day > dim)
        return;

    _clock->datetime.day = day;
    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Set_Day

void Mc_Clock_Set_Month(void *clock, uint8_t month)
{
    if (month > 12 || month == 0)
        return;

    clock_t *_clock = clock;
    _clock->datetime.month = month;

    // verify day in month
    uint8_t dim = days_in_month(_clock->datetime.month, _clock->datetime.year);
    if (_clock->datetime.day > dim)
    {
        (_clock->datetime.day) = dim;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Set_Month

void Mc_Clock_Set_Year(void *clock, uint16_t year)
{
    if(year > 2036 || year < 1901)
        return;

    clock_t *_clock = clock;
    _clock->datetime.year = year;

    // verify day in month
    uint8_t dim = days_in_month(_clock->datetime.month, _clock->datetime.year);
    if (_clock->datetime.day > dim)
    {
        (_clock->datetime.day) = dim;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Set_Year

void Mc_Clock_Set_Date(void *clock, uint16_t year, uint8_t month, uint8_t day)
{
    Mc_Clock_Set_Year(clock, year);
    Mc_Clock_Set_Month(clock, month);
    Mc_Clock_Set_Day(clock, day);
}// end Mc_Clock_Set_Date




// ==================   Getters   ================ //

int32_t Mc_Clock_Get_Timestamp(void *clock)
{
    clock_t *_clock = clock;
    return _clock->timestamp;
}// end Mc_Clock_Get_Timestamp

uint8_t Mc_Clock_Get_Second(void *clock)
{
    return ((clock_t *)clock)->datetime.second;
}// end Mc_Clock_Get_Second

uint8_t Mc_Clock_Get_Minute(void *clock)
{
    return ((clock_t *)clock)->datetime.minute;
}// end Mc_Clock_Get_Minute

uint8_t Mc_Clock_Get_Hour(void *clock)
{
    return ((clock_t *)clock)->datetime.hour;
}// end Mc_Clock_Get_Hour

uint8_t Mc_Clock_Get_Day(void *clock)
{
    return ((clock_t *)clock)->datetime.day;
}// end Mc_Clock_Get_Day

uint8_t Mc_Clock_Get_Month(void *clock)
{
    return ((clock_t *)clock)->datetime.month;
}// end Mc_Clock_Get_Month

uint16_t Mc_Clock_Get_Year(void *clock)
{
    return ((clock_t *)clock)->datetime.year;
}// end Mc_Clock_Get_Year




// ==================   Incrementers   ================ //

void Mc_Clock_Increment_Timestamp(void *clock)
{
    clock_t *_clock = clock;

    (_clock->timestamp)++;
    _clock->datetime = Mc_Clock_Timestamp_To_Human_Date(_clock->timestamp);
}// end Mc_Clock_Increment_Timestamp

void Mc_Clock_Increment_Timestamp_Value(void * clock, int32_t value)
{
    clock_t *_clock = clock;

    (_clock->timestamp) += value;
    _clock->datetime = Mc_Clock_Timestamp_To_Human_Date(_clock->timestamp);
}// end Mc_Clock_Increment_Timestamp_Value

void Mc_Clock_Increment_Second(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.second == 59)
    {
        _clock->datetime.second = 0;
    }
    else
    {
        (_clock->datetime.second)++;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Increment_Second

void Mc_Clock_Increment_Minute(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.minute == 59)
    {
        _clock->datetime.minute = 0;
    }
    else
    {
        (_clock->datetime.minute)++;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Increment_Minute

void Mc_Clock_Increment_Hour(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.hour == 23)
    {
        _clock->datetime.hour = 0;
    }
    else
    {
        (_clock->datetime.hour)++;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Increment_Hour

void Mc_Clock_Increment_Day(void *clock)
{
    clock_t *_clock = clock;

    // get days in the month
    uint8_t dim = days_in_month(_clock->datetime.month, _clock->datetime.year);

    if (_clock->datetime.day == dim)
    {
        _clock->datetime.day = 1;
    }
    else
    {
        (_clock->datetime.day)++;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Increment_Day

void Mc_Clock_Increment_Month(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.month == 12)
    {
        _clock->datetime.month = 1;
    }
    else
    {
        (_clock->datetime.month)++;
    }

    // verify days in month
    uint8_t dim = days_in_month(_clock->datetime.month, _clock->datetime.year);

    if (_clock->datetime.day > dim)
    {
        _clock->datetime.day = dim;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Increment_Month

void Mc_Clock_Increment_Year(void *clock)
{
    clock_t *_clock = clock;

    (_clock->datetime.year)++;

    // verify days in month (leap year)
    uint8_t dim = days_in_month(_clock->datetime.month, _clock->datetime.year);

    if (_clock->datetime.day > dim)
    {
        _clock->datetime.day = dim;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Increment_Year




// ==================   Decrementers   ================ //

void Mc_Clock_Decrement_Timestamp(void *clock)
{
    clock_t *_clock = clock;

    (_clock->timestamp)--;
    _clock->datetime = Mc_Clock_Timestamp_To_Human_Date(_clock->timestamp);
}// end Mc_Clock_Decrement_Timestamp

void Mc_Clock_Decrement_Timestamp_Value(void * clock, int32_t value)
{
    clock_t *_clock = clock;

    (_clock->timestamp) -= value;
    _clock->datetime = Mc_Clock_Timestamp_To_Human_Date(_clock->timestamp);
}// end Mc_Clock_Decrement_Timestamp_Value

void Mc_Clock_Decrement_Second(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.second == 0)
    {
        _clock->datetime.second = 59;
    }
    else
    {
        (_clock->datetime.second)--;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Decrement_Second

void Mc_Clock_Decrement_Minute(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.minute == 0)
    {
        _clock->datetime.minute = 59;
    }
    else
    {
        (_clock->datetime.minute)--;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Decrement_Minute

void Mc_Clock_Decrement_Hour(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.hour == 0)
    {
        _clock->datetime.hour = 23;
    }
    else
    {
        (_clock->datetime.hour)--;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Decrement_Hour

void Mc_Clock_Decrement_Day(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.day == 1)
    {
        // get days in month
        uint8_t dim = days_in_month(_clock->datetime.month, _clock->datetime.year);
        _clock->datetime.day = dim;
    }
    else
    {
        (_clock->datetime.day)--;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Decrement_Day

void Mc_Clock_Decrement_Month(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.month == 1)
    {
        _clock->datetime.month = 12;
    }
    else
    {
        (_clock->datetime.month)--;
    }

    // verify days in month
    uint8_t dim = days_in_month(_clock->datetime.month, _clock->datetime.year);
    if (_clock->datetime.day > dim)
    {
        _clock->datetime.day = dim;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Decrement_Month

void Mc_Clock_Decrement_Year(void *clock)
{
    clock_t *_clock = clock;

    if (_clock->datetime.year == 1970)
    {
        _clock->datetime.year = 1970;
    }
    else
    {
        (_clock->datetime.year)--;
    }

    // verify days in month
    uint8_t dim = days_in_month(_clock->datetime.month, _clock->datetime.year);
    if (_clock->datetime.day > dim)
    {
        _clock->datetime.day = dim;
    }

    // update timestamp
    _clock->timestamp = Mc_Clock_Human_Date_To_Timestamp(&(_clock->datetime));
}// end Mc_Clock_Decrement_Year
