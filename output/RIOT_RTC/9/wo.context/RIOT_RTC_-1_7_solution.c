#include <thread.h>
#include <stdio.h>
#include <xtimer.h>
#include <time.h>
#include <timex.h>
#include <periph/rtc.h>

static void alarm_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    struct tm time;

    // Set the RTC time to June 1, 2024
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // Months since January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("RTC time set failed!\n");
        return 1;
    }

    // Set an alarm 5 seconds from now
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time structure

    if (rtc_set_alarm(&alarm_time, alarm_cb, NULL) != 0) {
        printf("RTC alarm setup failed!\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now.\n");

    while (1) {
        // Keeping the main loop running
        xtimer_sleep(1);
    }

    return 0;
}