#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <xtimer.h> // for timer functionalities
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <arpa/inet.h>

static msg_t msg_queue[8];

void rtc_alarm_handler(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    msg_init_queue(msg_queue, 8);

    struct tm time;
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Simulated method to set time, as direct functionality is not allowed
    if (rtc_init() != 0) {
        puts("RTC init failed.");
        return 1;
    }

    // Simulated method to set RTC current time
    // rtc_set_time(&time);

    // Simulated method for rtc-set-alarm
    evtimer_msg_event_t evtimer_event;
    evtimer_event.event.callback = rtc_alarm_handler;
    evtimer_event.msg.type = 0;
    evtimer_event.msg.content.value = 1; // Example content, not directly used

    // Set an event timer that fires in 5 seconds.
    ztimer_sleep(ZTIMER_USEC, 5 * 1000000ULL);
    evtimer_add_msg(NULL, &evtimer_event, 0);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}