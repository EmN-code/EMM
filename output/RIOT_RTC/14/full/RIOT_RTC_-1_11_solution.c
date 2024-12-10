#include <ztimer.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <timex.h>
#include <sys/socket.h>
#include <stdio.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <event/callback.h>
#include <event/thread.h>
#include <mbox.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <thread.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <periph/gpio.h>

#define ALARM_SECONDS 5

typedef void (*rtc_alarm_cb_t)(void *arg);

static void alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 6 - 1; // tm_mon is months since January (0-11)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    while (1) {
        int rc = rtc_init();
        if (rc != 0) {
            puts("RTC initialization failed, retrying...");
            xtimer_sleep(1);
            continue;
        }
        puts("RTC initialized successfully");
        break;
    }

    while (1) {
        int rc = rtc_set_time(&time);
        if (rc != 0) {
            puts("Failed to set RTC time, retrying...");
            xtimer_sleep(1);
            continue;
        }
        puts("RTC time set successfully");
        break;
    }

    time.tm_sec += ALARM_SECONDS;

    while (1) {
        int rc = rtc_set_alarm(&time, alarm_callback, NULL);
        if (rc != 0) {
            puts("Failed to set RTC alarm, retrying...");
            xtimer_sleep(1);
            continue;
        }
        puts("RTC alarm set successfully");
        break;
    }

    // Infinite loop to keep the program running to test the alarm callback.
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}