#include <ztimer.h>
#include <xtimer.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <periph/rtc.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mutex.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <stddef.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <stdint.h>
#include <time_units.h>
#include <mbox.h>

#define ALARM_INTERVAL_SEC 5

static void callback(void *arg)
{
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize message box and RTC
    mbox_t mbox;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Set up the RTC to trigger after 5 seconds
    struct tm timeinfo = {0};
    timeinfo.tm_year = 2024 - 1900; // Year is year since 1900
    timeinfo.tm_mon = 5; // Month, where 0 = January
    timeinfo.tm_mday = 1; // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    rtc_set_time(&timeinfo);

    ztimer_now_t now = ztimer_now(ZTIMER_SEC);
    ztimer_t alarm;
    ztimer_set_msg(ZTIMER_SEC, &alarm, ALARM_INTERVAL_SEC * US_PER_SEC, sched_active_thread_pid);

    // Wait for the alarm to trigger
    msg_t msg;
    while (1) {
        if (msg_receive(&msg)) {
            callback(NULL);
        }
    }

    return 0;
}