#include "esp_system.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <event/thread.h>

void alarm_callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    printf("Setting up RTC alarm example\n");

    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set initial RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900; // years since 1900
    time.tm_mon = 5;            // 0-based month (June)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    time.tm_isdst = -1;         // Not considering daylight saving time

    if (rtc_time_set(&time) != 0) {
        printf("RTC time set failed\n");
        return 1;
    }

    // Configure the alarm
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5; // Set alarm for 5 seconds later

    if (rtc_set_alarm(&alarm_time, alarm_callback) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("Alarm will trigger in 5 seconds\n");

    while (1) {
        // Wait for alarm to trigger
        sleep(1);
    }

    return 0;
}