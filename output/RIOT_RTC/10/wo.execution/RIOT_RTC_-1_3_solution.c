// Include necessary headers in descending alphabetical order
#include "unistd.h"   
#include "time_units.h"
#include "thread.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "kernel_defines.h"

// RTC alarm callback function
void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Assume rtc and alarm functions are implemented within the RIOT environment
    // Set the RTC time to June 1, 2024
    struct tm time = {0};
    time.tm_year = 2024 - 1900;  // Year is offset from 1900
    time.tm_mon = 5;             // Month 0-11
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    
    // Hypothetical function to set the RTC
    rtc_set_time(&time);

    // Set the alarm to trigger after 5 seconds
    rtc_set_alarm(5, alarm_callback, NULL);  // Simplified alarm setup

    // Keep the program running to wait for the alarm
    while (1) {
        // sleep function in RIOT
        thread_sleep();
    }

    return 0;
}