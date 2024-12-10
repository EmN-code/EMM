
#include "evtimer_msg.h"
#include "stdio.h"
#include "netinet/in.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "timex.h"
#include "stdatomic.h"
#include "periph/i2c.h"
#include "xtimer.h"
#include "unistd.h"
#include "periph/gpio.h"
#include "time_units.h"
#include "stdbool.h"
#include "msg.h"
#include "periph/rtc.h"
#include "mbox.h"
#include "stddef.h"
#include "ztimer.h"

#define ALARM_DELAY_SEC (5)

static evtimer_msg_t alarm_timer;
static msg_t msg_queue[8];

void alarm_callback(void *arg)
{
    (void)arg;
    puts("Alarm triggered!");
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&alarm_timer);

    // Set the alarm to trigger after 5 seconds
    evtimer_msg_event_t alarm_event;
    alarm_event.event.handler = alarm_callback;
    alarm_event.event.arg = NULL;
    xtimer_t alarm_timer_xtimer;
    alarm_timer_xtimer.target = xtimer_now() + (ALARM_DELAY_SEC * US_PER_SEC);
    alarm_timer_xtimer.long_target = alarm_timer_xtimer.target;
    alarm_timer_xtimer.callback = (void (*)(void *))evtimer_add_msg;
    alarm_timer_xtimer.arg = &alarm_event;
    xtimer_set(&alarm_timer_xtimer, ALARM_DELAY_SEC * US_PER_SEC);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == EVTIMER_MSG_EVENT) {
            evtimer_msg_event_t *event = (evtimer_msg_event_t *)msg.content.ptr;
            event->event.handler(event->event.arg);
        }
    }

    return 0;
}
