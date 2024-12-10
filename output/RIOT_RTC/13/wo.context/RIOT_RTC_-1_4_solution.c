#include <xtimer.h>
#include <stdio.h>
#include "event/callback.h"
#include "periph/gpio.h"
#include "msg.h"

void rtc_alarm(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Set the RTC time to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 5; // June (0 indexed, so 5 is June)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1; // No daylight saving time
    set_time(&time);

    // Set an alarm 5 seconds later
    xtimer_sleep(5);
    rtc_alarm();

    return 0;
}