#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "msg.h"
#include "unistd.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "stdatomic.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "evtimer_msg_event_init"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

#define ALARM_INTERVAL_SEC 5

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Step 1: Initialize RTC
    rtc_init();

    // Step 2: Set RTC time to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month 0-11
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    // Step 3: Initiate rtc alarm
    struct tm alarm_time = time;
    alarm_time.tm_sec += ALARM_INTERVAL_SEC;

    rtc_alarm_cb_t cb = alarm_callback;

    int rc;
    while(1) {
        rc = rtc_set_alarm(&alarm_time, cb, NULL);
        if(rc == 0) {
            printf("Alarm set successfully!\n");
            break;
        } else {
            printf("Failed to set alarm, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
    }

    while (1) {
        msg_t msg;
        msg_receive(&msg); // Block until a message is received, simulating waiting for RTC interrupt
    }

    return 0;
}