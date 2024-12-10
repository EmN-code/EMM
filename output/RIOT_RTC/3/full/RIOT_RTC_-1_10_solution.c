#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdatomic.h>
#include <assert.h>
#include <arpa/inet.h>
#include <msg.h>
#include <mutex.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <xtimer.h>
#include <ztimer.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <dht.h>
#include <dht_params.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

void alarm_callback(void *arg) {
    puts("Alarm triggered!");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the current time to a specific date (June 1, 2024, 00:00:00)
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // Month since January (0-11)
    time.tm_mday = 1;           // Day of the month (1-31)
    time.tm_hour = 0;           // Hours since midnight (0-23)
    time.tm_min = 0;            // Minutes after the hour (0-59)
    time.tm_sec = 0;            // Seconds after the minute (0-59)

    // Set custom time (RTC)
    if (rtc_set_time(&time) != 0) {
        puts("Failed to set time.");
        return -1;
    }

    // Set alarm time 5 seconds from now
    time.tm_sec += 5;

    // Normalize the time after modifying seconds
    mktime(&time);

    // Set an alarm with a callback
    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        puts("Failed to set alarm.");
        return -1;
    }

    // Keep running
    while (1) {
        thread_yield();
    }

    return 0;
}
