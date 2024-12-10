#include <time.h>  // For RTC time management
#include <stdio.h> // For printf function
#include <kernel.h> // Includes kernel functions for RIOT

// Global variable for RTC context
rtc_t rtc;

// Callback function for RTC alarm
void rtc_alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;

    // Initialize RTC and set RTC time and date to June 1, 2024
    rtc_init(&rtc);
    timeinfo.tm_year = 2024 - 1900;  // Years since 1900
    timeinfo.tm_mon = 5;     // Months since January [0-11]
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    rtc_set_time(&rtc, &timeinfo);
    
    // Set RTC alarm for 5 seconds later
    timeinfo.tm_sec += 5;
    rtc_set_alarm(&rtc, &timeinfo, rtc_alarm_callback, NULL);

    // Run the main loop
    while(1) {
        // Idle loop to keep the program running
        xtimer_sleep(1);
    }

    return 0;
}