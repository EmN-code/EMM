#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>  // Standard integer types
#include <periph/rtc.h>

void alarm_triggered(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_to_set;
    time_to_set.tm_year = 2024 - 1900; // Year since 1900
    time_to_set.tm_mon = 5;  // June (months since January, 0-11)
    time_to_set.tm_mday = 1; // 1st of the month
    time_to_set.tm_hour = 0;
    time_to_set.tm_min = 0;
    time_to_set.tm_sec = 0;

    // Set the RTC time
    if (rtc_set_time(&time_to_set) != 0) {
        printf("Failed to set the RTC time\n");
        return 1;
    }

    printf("RTC set to June 1, 2024, 00:00:00\n");

    // Sleep for 5 seconds to simulate an alarm.
    ztimer_sleep(ZTIMER_MSEC_BASE, 5000);

    // Trigger the alarm callback
    alarm_triggered();

    return 0;
}