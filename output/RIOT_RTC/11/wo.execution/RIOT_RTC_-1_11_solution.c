#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <msg.h>
#include <ztimer.h>
#include <timex.h>
#include <periph/rtc.h>
#include <esp_system.h>
#include <bool.h>

#define ALARM_SECONDS 5

static void alarm_callback(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    rtc_init(); // Initialize the RTC subsystem
    
    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5; // 0-based index for June
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(mktime(&time));

    // Set an alarm for 5 seconds from the current time
    rtc_set_alarm_reltimer(ALARM_SECONDS, alarm_callback, NULL);

    while (1) {
        // Wait indefinitely, alarm_callback will be called when the alarm triggers
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}