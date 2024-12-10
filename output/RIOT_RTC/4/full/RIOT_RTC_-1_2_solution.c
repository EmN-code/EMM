#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "periph/rtc.h"

#include <stdio.h>

void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;
    struct tm alarm_time;

    rtc_init();

    // Setting the date to June 1, 2024
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // Month 0 - 11
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm for 5 seconds later
    alarm_time = time;
    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    while (1) {
        xtimer_sleep(1); // Let the mcu sleep and wait for events (e.g., alarms)
    }

    return 0;
}