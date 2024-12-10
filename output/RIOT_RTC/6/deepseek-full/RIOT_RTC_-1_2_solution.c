
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"

#define ALARM_PIN GPIO_PIN(0, 13)

void alarm_callback(void *arg) {
    (void)arg;
    gpio_set(ALARM_PIN);
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize GPIO for alarm indication
    gpio_init(ALARM_PIN, GPIO_OUT);
    gpio_clear(ALARM_PIN);

    // Initialize RTC
    if (rtc_init() < 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // years since 1900
    time.tm_mon = 5;            // months since January (0-11)
    time.tm_mday = 1;           // day of the month (1-31)
    time.tm_hour = 0;           // hours since midnight (0-23)
    time.tm_min = 0;            // minutes after the hour (0-59)
    time.tm_sec = 0;            // seconds after the minute (0-59)

    if (rtc_set_time(&time) < 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set an alarm after 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) < 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    // Main loop to keep the program running
    while (1) {
        // Do nothing, wait for the alarm to trigger
    }

    return 0;
}
