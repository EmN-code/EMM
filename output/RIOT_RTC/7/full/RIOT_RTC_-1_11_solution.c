#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stddef.h>
#include <sys/socket.h>
#include <unistd.h>
#include <msg.h>
#include <mutex.h>
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "timex.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "kernel_defines.h"
#include "dht_params.h"
#include "dht.h"
#include "time_units.h"
#include "mbox.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int init_rtc_with_alarm(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // Months since January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    while (1) {
        int rc = rtc_init();
        if (rc != 0) {
            printf("RTC initialization failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("RTC initialized successfully.\n");
            break;
        }
    }

    rtc_set_time(&time);

    // Set an alarm 5 seconds from now
    struct tm alarm_time;
    alarm_time.tm_year = 2024 - 1900;
    alarm_time.tm_mon = 5;
    alarm_time.tm_mday = 1;
    alarm_time.tm_hour = 0;
    alarm_time.tm_min = 0;
    alarm_time.tm_sec = 5;

    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    return 0;
}

int main(void) {
    printf("Starting RTC with alarm example\n");
    init_rtc_with_alarm();
    while(1) {
        xtimer_sleep(60); // Sleep to keep the program alive
    }
    return 0;
}