#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
#include "rtc.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "dht.h"
#include "dht_params.h"
#include "assert.h"
#include "msg.h"
#include "mutex.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "arpa/inet.h"

#define ALARM_INTERVAL_SEC 5

// Function to set the RTC time
static int rtc_set_custom_time(struct tm *time) {
    return rtc_set_time((time_t)mktime(time));
}

// Callback for when the RTC alarm is triggered
static void rtc_alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        puts("RTC initialization failed!");
        return 1;
    }

    // Set the RTC time to June 1, 2024
    struct tm time;
    memset(&time, 0, sizeof(struct tm));
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // Month, 0 = January
    time.tm_mday = 1;           // Day of the month

    if (rtc_set_custom_time(&time) < 0) {
        puts("Failed to set RTC time!");
        return 1;
    }

    // Calculate alarm time
    time_t alarm_time = mktime(&time) + ALARM_INTERVAL_SEC;
    struct tm alarm_tm;
    gmtime_r(&alarm_time, &alarm_tm);

    // Set the RTC alarm
    rtc_set_alarm((time_t)alarm_time, rtc_alarm_callback, NULL);

    // Sleep and wait for the alarm to trigger
    puts("Alarm set. Waiting...");
    xtimer_sleep(ALARM_INTERVAL_SEC + 2); // Sleep a bit longer than the alarm interval

    return 0;
}