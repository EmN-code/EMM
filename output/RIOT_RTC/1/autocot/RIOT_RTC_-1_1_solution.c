// Description of the problem and approach:
// This code sets an RTC alarm on the ESP32 using RIOT-OS features. 
// It configures the RTC to a specific date and time, and it sets an alarm to trigger after 5 seconds.
// When the alarm is activated, it will print "Alarm triggered!".

#include <stdio.h>
#include <time.h>
#include <rtc.h>

void alarm_callback(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    // Set date to June 1, 2024
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // Months since January (0-11)
    time.tm_mday = 1;           // Day of the month (1-31)
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC
    rtc_init();

    // Set RTC time
    rtc_set_time(&time);

    // Prepare the alarm time
    struct tm alarm_time = time;
    // Set the alarm to trigger after 5 seconds
    alarm_time.tm_sec += 5;

    // Set RTC alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Enter an infinite loop, waiting for the alarm to trigger
    while (1) {
        // Usually, you'd put the MCU to sleep here
        // For simulation purposes, we'll keep it in a busy loop
    }

    return 0;
}