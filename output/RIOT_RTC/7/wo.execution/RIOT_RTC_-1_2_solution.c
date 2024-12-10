#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

static void rtc_cb(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Set the RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month 0 - 11
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    // Set an RTC alarm for 5 seconds later
    struct tm alarm_time;
    alarm_time.tm_year = 2024 - 1900;
    alarm_time.tm_mon = 6 - 1;
    alarm_time.tm_mday = 1;
    alarm_time.tm_hour = 0;
    alarm_time.tm_min = 0;
    alarm_time.tm_sec = 5;

    rtc_set_alarm(&alarm_time, rtc_cb, NULL);

    // Infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}