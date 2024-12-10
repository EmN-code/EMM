#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <periph/rtc.h>
#include <ztimer.h>
#include <assert.h>
#include <paho_mqtt.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <msg.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <net/emcute.h>
#include <mutex.h>
#include <periph/i2c.h>
#include <time_units.h>
#include <periph/gpio.h>

// Callback function for the RTC alarm
void alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return -1;
    }

    // Set the RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Assuming rtc_set_time is a valid function in the context of RIOT
    if (rtc_time_set(&time) != 0) {
        printf("Failed to set RTC time!\n");
        return -1;
    }

    // Set an alarm 5 seconds from now
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    // Set the callback for the alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Sleep until the alarm triggers
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}