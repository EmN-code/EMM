#include <stdio.h>
#include <time.h>
#include <ztimer.h>  // Corrected name from xtimer to ztimer.
#include <periph/rtc.h>  // Ensure the correct peripheral library is included.

#define YEAR 2024
#define MONTH 6
#define DAY 1
#define HOUR 0
#define MINUTE 0
#define SECOND 0
#define ALARM_DELAY 5  // Alarm after 5 seconds

static void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = YEAR - 1900;  // tm_year is years since 1900
    time.tm_mon = MONTH - 1;     // tm_mon is 0-11
    time.tm_mday = DAY;
    time.tm_hour = HOUR;
    time.tm_min = MINUTE;
    time.tm_sec = SECOND;

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
        return 1;
    }

    // Set the alarm to 5 seconds after the set time
    time.tm_sec += ALARM_DELAY;
    ztimer_sleep(ZTIMER_SEC, ALARM_DELAY);  // Synchronize delay with alarm

    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    while(1) {  // Keep the program running
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;  // Not reached
}