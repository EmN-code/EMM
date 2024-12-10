#include <xtimer.h>
#include <time.h>
#include <periph/gpio.h>
#include <periph/rtc.h>

void rtc_alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Define time structure with desired date: June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900,  // RTC uses years since 1900
        .tm_mon = 5,    // June is month 5 (January is 0)
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize RTC
    if (rtc_init() != 0) {
        puts("RTC initialization failed");
        return 1;
    }

    // Set RTC time
    if (rtc_set_time(&time) != 0) {
        puts("RTC time setting failed");
        return 1;
    }

    // Define alarm time: 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Adjust struct for a valid time

    // Set RTC alarm
    if (rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL) != 0) {
        puts("RTC alarm setting failed");
        return 1;
    }

    puts("RTC time set, waiting for alarm...");

    while (1) {
        // Stay in low power mode waiting for alarm
        xtimer_sleep(10);
    }

    return 0;
}