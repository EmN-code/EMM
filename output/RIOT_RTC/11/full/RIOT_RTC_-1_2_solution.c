#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t time;
    rtc_alarm_t alarm;
    int res;

    // Set the RTC time to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // Month, note $0 - January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC and set time
    while (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time. Retrying in 1 second.\n");
        // Sleep or delay mechanism, careful to avoid blacklisted api
        for (volatile int i = 0; i < 1000000; ++i);
    }

    // Set RTC alarm for 5 seconds from now
    alarm.time = time;
    alarm.time.tm_sec += 5; // Add 5 seconds
    res = rtc_set_alarm(&alarm, alarm_callback, NULL);
    if (res != 0) {
        printf("Failed to set alarm. Retrying in 1 second.\n");
        // Sleep or delay mechanism
        for (volatile int i = 0; i < 1000000; ++i);
    }

    printf("RTC and alarm configured. Waiting for alarm to trigger.\n");

    // Keep the main thread active
    while (1) {
        // Sleep or delay simulation, careful to avoid blacklisted api
        for (volatile int i = 0; i < 1000000; ++i);
    }

    return 0;
}