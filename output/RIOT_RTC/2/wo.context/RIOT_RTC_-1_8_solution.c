#include <stdio.h>
#include <errno.h>
#include "esp_idf_rtc_gpio_pullup_en.h"
#include "esp_idf_gpio_set_intr_type.h"

// RTC alarm callback function
void rtc_alarm_callback(void *arg) {
    (void) arg; // Ensure the compiler does not warn about unused variable
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC (you should replace this with an actual RTC initialization)
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return -1;
    }

    // Set date: June 1, 2024 (YYYY, MM, DD, HH, MM, SS)
    struct tm date = {
        .tm_year = 2024 - 1900, // tm_year is year since 1900
        .tm_mon = 6 - 1,        // tm_mon is 0-based
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&date) != 0) {
        printf("Failed to set date and time!\n");
        return -1;
    }

    // Set alarm after 5 seconds
    rtc_alarm_cb_t alarm_cb;
    alarm_cb = rtc_alarm_callback;
    if (rtc_set_alarm_in_seconds(alarm_cb, 5, NULL) != 0) {
        printf("Failed to set RTC alarm!\n");
        return -1;
    }

    printf("RTC Alarm set for 5 seconds from now. Waiting for alarm to trigger...\n");

    // Infinite loop to keep main() executing so callback can be triggered
    while (1) {
        // Typically you'd have low-power sleep here
    }

    return 0;
}