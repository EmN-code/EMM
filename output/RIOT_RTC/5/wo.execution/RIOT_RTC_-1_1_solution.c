#include "timex.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "stdio.h"

void alarm_callback(void *arg) {
    (void)arg;  // Argument is unused
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the date and time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // Month 0-11 (6th month - June)
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Print current time
    printf("Current time set to: %04d-%02d-%02d %02d:%02d:%02d\n",
            time.tm_year + 1900, time.tm_mon + 1, time.tm_mday,
            time.tm_hour, time.tm_min, time.tm_sec);

    // Set the alarm to 5 seconds later
    time.tm_sec += 5; // Add 5 seconds
    mktime(&time); // Normalize the time structure
    rtc_set_alarm(&time, alarm_callback, NULL);
    
    // Put the CPU to sleep to wait for the alarm
    printf("Waiting for alarm...\n");
    while (1) {
        // Busy wait, or use a low-power wait if applicable
    }

    return 0;
}