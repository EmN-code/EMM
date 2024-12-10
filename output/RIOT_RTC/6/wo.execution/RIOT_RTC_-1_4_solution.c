#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mbox.h>
#include <msg.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <timex.h>
#include <unistd.h>
#include <ztimer.h>
#include <time_units.h>

#define ALARM_DURATION (5U)

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    evtimer_t alarm_timer;
    evtimer_msg_event_t alarm_event;
    msg_t m;
    int64_t set_time_sec = 1719907200; // Equivalent to June 1, 2024, in POSIX time

    printf("Setting RTC to June 1, 2024\n");

    evtimer_msg_init(&alarm_event, &m);
    evtimer_set_msg(&alarm_timer, &alarm_event, ALARM_DURATION);
    evtimer_set_cb(&alarm_timer, alarm_callback, NULL);

    while (1) {
        msg_receive(&m);  // Wait for alarm to trigger
    }

    return 0;
}