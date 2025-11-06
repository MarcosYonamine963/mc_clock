/**
 * @file mc_clock.h
 * @author Marcos Yonamine
 * @date 06/nov/2025
 * @version v1.0.0
 * 
 * Example of usage:

    int main(void)
    {
        void * clock = Mc_Clock_New();

        int32_t timestamp = 1762458942;
        Mc_Clock_Set_Timestamp(clock, timestamp);

        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;

        year = Mc_Clock_Get_Year(clock);
        month = Mc_Clock_Get_Month(clock);
        day = Mc_Clock_Get_Day(clock);
        hour = Mc_Clock_Get_Hour(clock);
        minute = Mc_Clock_Get_Minute(clock);
        second = Mc_Clock_Get_Second(clock);

        printf("%d/%d/%ld %d:%d:%d\r\n", day, month, year, hour, minute, second);

        Mc_Clock_Destroy(clock);

        return 0;
    }
 */

#ifndef _MC_CLOCK_H
#define _MC_CLOCK_H

#ifdef __cplusplus
extern "C" 
{
#endif

#include <stdint.h>


// ==================   Object Manipulation   ================ //

/**
 * @brief Creates an Clock Object and returns a pointer to it
 * 
 */
void * Mc_Clock_New(void);

/**
 * @brief Creates a copy of the object clock and returns a pointer to it
 * 
 */
void * Mc_Clock_Clone(void * clock);

/**
 * @brief Free memory space of the object clock
 * 
 */
void Mc_Clock_Destroy(void * clock);




// ==================   Clearers   ================ //

/**
 * @brief Set time to 12:00:00 AM. Dont change date.
 */
void Mc_Clock_Clear_Time(void * clock);

/**
 * @brief Set day, month, year and time to 1/jan/2020 12:00:00 AM
 * @note Set epoch timestamp to 1577836800
 */
void Mc_Clock_Clear_DateTime(void * clock);




// ==================   Setters   ================ //

/**
 * @brief Set epoch timestamp (seconds since 1/jan/1970 12:00:00 AM)
 * 
 */
void Mc_Clock_Set_Timestamp(void * clock, int32_t timestamp);

/**
 * @brief Set clock seconds value and update timestamp
 * 
 */
void Mc_Clock_Set_Second(void * clock, uint8_t second);

/**
 * @brief Set clock minutes value and update timestamp
 * 
 */
void Mc_Clock_Set_Minute(void * clock, uint8_t minute);

/**
 * @brief Set clock hour value and update timestamp
 * 
 */
void Mc_Clock_Set_Hour(void * clock, uint8_t hour);

/**
 * @brief Set clock time: hour, minute, second and update timestamp
 * 
 */
void Mc_Clock_Set_Time(void * clock, uint8_t hour, uint8_t minute, uint8_t second);

/**
 * @brief Set datetime day value and update timestamp
 * 
 */
void Mc_Clock_Set_Day(void * clock, uint8_t day);

/**
 * @brief Set clock month value and update timestamp
 * 
 */
void Mc_Clock_Set_Month(void * clock, uint8_t month);

/**
 * @brief Set clock year value and update timestamp
 */
void Mc_Clock_Set_Year(void * clock, uint16_t year);

/**
 * @brief Set clock date: year, month, day and update timestamp
 * 
 */
void Mc_Clock_Set_Date(void * clock, uint16_t year, uint8_t month, uint8_t day);




// ==================   Getters   ================ //

/**
 * @brief Get epoch timestamp (seconds since to 1/jan/1970 12:00:00 AM)
 * 
 */
int32_t Mc_Clock_Get_Timestamp(void * clock);

/**
 * @brief Get clock second value
 * 
 */
uint8_t Mc_Clock_Get_Second(void * clock);

/**
 * @brief Get clock minute value
 */
uint8_t Mc_Clock_Get_Minute(void * clock);

/**
 * @brief Get clock hour value
 * 
 */
uint8_t Mc_Clock_Get_Hour(void * clock);

/**
 * @brief Get clock day value
 * 
 */
uint8_t Mc_Clock_Get_Day(void * clock);

/**
 * @brief Get clock month value
 * 
 */
uint8_t Mc_Clock_Get_Month(void * clock);

/**
 * @brief Get clock year value
 * 
 */
uint16_t Mc_Clock_Get_Year(void * clock);




// ==================   Incrementers   ================ //

/**
 * @brief Increment clock timestamp in 1 second. Should modify all datetime.
 * 
 */
void Mc_Clock_Increment_Timestamp(void * clock);

/**
 * @brief Increment clock timestamp in <value> seconds. Should modify all datetime.
 * 
 */
void Mc_Clock_Increment_Timestamp_Value(void * clock, int32_t value);

/**
 * @brief Increment clock second. Dont modify other parameters (minutes, hour, etc)
 * 
 */
void Mc_Clock_Increment_Second(void * clock);

/**
 * @brief Increment clock minute. Dont modify other parameters (seconds, hour, etc)
 * 
 */
void Mc_Clock_Increment_Minute(void * clock);

/**
 * @brief Increment clock hour. Dont modify other parameters (minutes, days, etc)
 * 
 */
void Mc_Clock_Increment_Hour(void * clock);

/**
 * @brief Increment clock day. Dont modify other parameters (month, year, etc)
 * 
 */
void Mc_Clock_Increment_Day(void * clock);

/**
 * @brief Increment clock month. Only modify day if in this day doesn't exist in this month. <br>
 * Example. If day was 30, and month now is 2, day will be changed to 28 or 29 depending on leap year.
 * 
 */
void Mc_Clock_Increment_Month(void * clock);

/**
 * @brief Increment clock year. Only modify day if in this day doesn't exist in this month. <br>
 * Example. If day was 29, month 2 and year 2020, by incrementing year to 2021, day will be 28 (not leap year).
 * 
 */
void Mc_Clock_Increment_Year(void * clock);




// ==================   Decrementers   ================ //

/**
 * @brief Decrement clock timestamp in 1 second. Should modify all datetime.
 * 
 */
void Mc_Clock_Decrement_Timestamp(void * clock);

/**
 * @brief Decrement clock timestamp in <value> seconds. Should modify all datetime.
 * 
 */
void Mc_Clock_Decrement_Timestamp_Value(void * clock, int32_t value);

/**
 * @brief Decrement clock second. Dont modify other parameters (minute, hour, etc)
 * 
 */
void Mc_Clock_Decrement_Second(void * clock);

/**
 * @brief Decrement clock minute. Dont modify other parameters (second, hour, etc)
 * 
 */
void Mc_Clock_Decrement_Minute(void * clock);

/**
 * @brief Decrement clock hour. Dont modify other parameters (second, minute, etc)
 * 
 */
void Mc_Clock_Decrement_Hour(void * clock);

/**
 * @brief Decrement clock day. Dont modify other parameters (month, year, etc)
 * 
 */
void Mc_Clock_Decrement_Day(void * clock);

/**
 * @brief Decrement clock month. Only modify day if in this day doesn't exist in this month. <br>
 * Example. If day was 30, and month now is 2, day will be changed to 28 or 29 depending on leap year.
 * 
 */
void Mc_Clock_Decrement_Month(void * clock);

/**
 * @brief Decrement clock year. Only modify day if in this day doesn't exist in this month. <br>
 * Example. If day was 29, month 2 and year 2020, by decrementing year to 2019, day will be 28 (not leap year).
 * 
 */
void Mc_Clock_Decrement_Year(void * clock);


#ifdef __cplusplus
}
#endif

#endif /* _MC_CLOCK_H */
