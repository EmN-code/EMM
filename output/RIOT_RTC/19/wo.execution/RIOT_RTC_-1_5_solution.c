#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "unistd.h"

#define ALARM_SECONDS (5U)

// RTC alarm callback function
void alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

// Main function to initialize RTC, set time, and alarm
int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5; // Month, 0 = January
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC with the specified time
    rtc_set_time(&time);

    // Wait for the RTC to update
    xtimer_sleep(1);

    // Set an alarm to trigger after 5 seconds
    rtc_alarm_t alarm;
    rtc_get_time(&time); // Get current time
    time.tm_sec += ALARM_SECONDS; // Set for 5 seconds later
    mktime(&time); // Normalize time structure

    alarm.time.tm_sec = time.tm_sec;
    alarm.time.tm_min = time.tm_min;
    alarm.time.tm_hour = time.tm_hour;
    alarm.time.tm_mday = time.tm_mday;
    alarm.time.tm_mon = time.tm_mon;
    alarm.time.tm_year = time.tm_year;

    rtc_set_alarm(&alarm, alarm_cb, NULL);

    // Main loop
    while (1) {
        // The main thread can perform other tasks here
        xtimer_sleep(60); // Sleep for a minute
    }

    return 0;
}