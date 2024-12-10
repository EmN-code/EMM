#include "xtimer.h"
#include "periph/rtc.h"

int main(void) {
    // Set the RTC date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5; // Months since January [0-11]
    time.tm_mday = 1; // Day of the month [1-31]
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Configure the alarm to trigger in 5 seconds
    xtimer_sleep(5);

    // Alarm action
    puts("Alarm triggered!");
    return 0;
}