#include "mutex.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "atomic.h"
#include "stdio.h"
#include "ztimer.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "timex.h"
#include "unistd.h"
#include "zdtipe.h"
#include "time_units.h"
#include "xtimer.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "stdbool.h"
#include "sched.h"

#define RTC_BASE_YEAR 2023

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    kernel_pid_t target_pid = thread_getpid();
    evtimer_msg_event_t alarm_event = {0};
    evtimer_t evtimer = {0};

    // Set up the date to June 1, 2024
    struct tm timeinfo = {
        .tm_year = 2024 - RTC_BASE_YEAR,
        .tm_mon = 6 - 1,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Set the event timer callback
    evtimer_msg_init(&evtimer);
    evtimer_msg_event(&evtimer, &alarm_event, target_pid);
    evtimer_set_msg(&alarm_event.msg, &msg);

    // Set an alarm to trigger after 5 seconds
    evtimer_set_cb(&alarm_event.event, alarm_callback, NULL);
    evtimer_add(&evtimer, &alarm_event.event);

    // Wait here for the alarm to occur (simulate blocking)
    while (1) {
        msg_receive(&msg);
    }

    return 0;
}
