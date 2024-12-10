#include <stdio.h>
#include <rtc.h>
#include <timex.h>
#include <ztimer.h>

void alarm_callback(void *arg) {
    (void)arg;  // Avoid unused warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the current date and time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 5;             // Months since January (0-11)
    time.tm_mday = 1;            // Day of the month (1-31)
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set the alarm to trigger 5 seconds from the current time
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Wait here to prevent the program from exiting
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}