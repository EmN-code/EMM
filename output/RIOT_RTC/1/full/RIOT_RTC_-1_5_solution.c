#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <periph/rtc.h>
#include <ztimer.h>

// Function to configure and trigger the RTC alarm
void configure_rtc_alarm(void) {
    // Initialize the RTC
    rtc_init();

    // Set the date and time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 5;             // Months since January [0-11]
    time.tm_mday = 1;            // Day of the month [1-31]
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_time_set(&time);  // Set the RTC time

    // Configure the alarm time to 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    rtc_alarm_set(&alarm_time);  // Set the RTC alarm
}

// Alarm callback function
void alarm_triggered(void) {
    puts("Alarm triggered!");
}

int main(void) {
    configure_rtc_alarm();

    while (1) {
        // Sleep until the alarm interrupt wakes up the system
        xtimer_sleep(1);
    }

    // Attach the alarm callback
    rtc_set_alarm_cb(alarm_triggered);

    return 0;
}