#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "xtimer.h"

void rtc_alarm_callback(void)
{
    // This function will trigger when the RTC alarm is activated
    printf("Alarm triggered!\n");
}

int main(void)
{
    rtc_time_t time;
    rtc_alarm_t alarm;

    // Set the initial date and time to June 1, 2024, 00:00:00
    time.tm_year = 2024;
    time.tm_mon = 6;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize the RTC
    while(1) {
        if (rtc_set_time(&time) != 0) {
            printf("Failed to set RTC time! Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("RTC time set successfully.\n");
            break;
        }
    }

    // Set the alarm for 5 seconds later
    alarm.target.year = time.tm_year;
    alarm.target.month = time.tm_mon;
    alarm.target.day = time.tm_mday;
    alarm.target.hour = time.tm_hour;
    alarm.target.minute = time.tm_min;
    alarm.target.second = time.tm_sec + 5;
    alarm.callback = rtc_alarm_callback;

    // Initialize the RTC alarm
    while(1) {
        if (rtc_set_alarm(&alarm) != 0) {
            printf("Failed to set RTC alarm! Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("RTC alarm set successfully.\n");
            break;
        }
    }

    // Keep the program running
    while (1){
        xtimer_sleep(1); // Sleep for a while to reduce CPU usage
    }

    return 0;
}