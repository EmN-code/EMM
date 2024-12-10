#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "time_units.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "kernel_defines.h"
#include "arpa/inet.h"

#define ALARM_OFFSET_SEC (5U)

static void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;
    memset(&time, 0, sizeof(struct tm));
    memset(&alarm_time, 0, sizeof(struct tm));

    // Set time to June 1, 2024
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5;   // months since January
    time.tm_mday = 1;

    evtimer_t evtimer;
    evtimer_mbox_event_t event;
    msg_t msg;
    mbox_t mbox;

    // Initialize mbox, evtimer etc.
    mbox_init(&mbox, &msg, 1);
    evtimer_init(&evtimer);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Set RTC time
    rtc_init();
    if (rtc_set_time(&time) != 0) {
        printf("Time setting failed.");
        return -1;
    }

    while (1) {
        if (rtc_get_time(&time) != 0) {
            printf("Failed to get time.");
            continue;
        }
        alarm_time = time;
        alarm_time.tm_sec += ALARM_OFFSET_SEC;
        if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != 0) {
            printf("Failed to set alarm.");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    // Wait for the alarm to trigger
    while (1) {
        // Do nothing, just wait and process messages if needed
        msg_receive(&msg);
    }

    return 0;
}