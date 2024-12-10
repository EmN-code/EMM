#include <ztimer.h>
#include <timex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <event/callback.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mbox.h>
#include <thread.h>
#include <timex.h>

#define ALARM_INTERVAL_SEC 5

atomic_bool alarm_triggered = false;

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
    atomic_store(&alarm_triggered, true);
}

int main(void) {
    // Initialize RTC
    rtc_t rtc;
    rtc_init(&rtc);

    // Set RTC time to June 1st, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 5; // June is the sixth month
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&rtc, &time);

    // Schedule alarm after ALARM_INTERVAL_SEC
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_msg_event_init(&event, alarm_callback, NULL);
    evtimer_add_msg(&evtimer, &event, thread_getpid());
    evtimer_set(&evtimer, (ztimer_now(ZTIMER_MSEC) + ALARM_INTERVAL_SEC * MS_PER_SEC));

    // Main loop, waiting for the alarm to be triggered
    while (!atomic_load(&alarm_triggered)) {
        thread_yield();
    }
    return 0;
}