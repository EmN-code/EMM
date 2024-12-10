#include <stdio.h>
#include <periph/gpio.h>

// Real Time Clock (RTC) initialization function
void rtc_init(void) {
    // Initialize RTC hardware and set the necessary configuration
}

// RTC set time function
void rtc_set_time(struct tm *timeinfo) {
    // Set the RTC time using the provided timeinfo struct
}

// RTC set alarm function
void rtc_set_alarm(struct tm *alarminfo, void (*callback)(void)) {
    // Set the alarm for the RTC using alarminfo struct
    // Register the callback function to be called when alarm triggers
}

// Callback function for when the alarm is triggered
void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;

    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 5;  // Month, where 0 = January
    timeinfo.tm_mday = 1; // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Initialize RTC
    rtc_init();

    // Set current time in RTC
    rtc_set_time(&timeinfo);

    // Set an alarm for 5 seconds after current time
    timeinfo.tm_sec += 5;

    // Set RTC Alarm
    rtc_set_alarm(&timeinfo, alarm_callback);

    // Keep the program running to catch the alarm callback
    while (1) {
        // Wait for the alarm to trigger
    }

    return 0;
}