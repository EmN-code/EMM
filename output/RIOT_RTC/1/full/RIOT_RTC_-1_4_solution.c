// In this code, we will utilize the RTC peripheral of an ESP32 to set the date and trigger an alarm.
// The main activity involves setting the RTC time to June 1, 2024, and then setting an alarm to trigger after 5 seconds.

#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht_params.h"
#include "arpa/inet.h"

// Function to initialize and set the RTC time
void initialize_rtc(void) {
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;  // Years since 1900
    timeinfo.tm_mon = 5;             // Months since January [0-11]
    timeinfo.tm_mday = 1;            // Day of the month [1-31]
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1;          // No daylight saving time

    // Initialize RTC
    rtc_init();
    // Set RTC time
    rtc_set_time(&timeinfo);
}

// Function to handle RTC alarm
void alarm_handler(void) {
    printf("Alarm triggered!\n");
}

// Function to setup the RTC alarm
void set_rtc_alarm(void) {
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;  // Set alarm for 5 seconds later

    rtc_set_alarm(&alarm_time, alarm_handler);
}

int main(void) {
    // Initialize RTC and set time to June 1, 2024
    initialize_rtc();
    // Set Alarm 5 seconds from the initialized time
    set_rtc_alarm();
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000); // Adding a sleep to allow time passage
    }
    return 0;
}