# Clock C Driver for embedded systems

## Features
- Simulate Object Orientation
- Usage with timestamps (signed int32_t value)
- Set and Get values of:
   - timestamp
   - year
   - month
   - day
   - hour
   - minute
   - second
 
- Increment/Decrement of values
- Increment/Decrement of timestamp

## Example of usage

```C
#include "stdio.h"
#include "mc_clock.h"

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
```
