
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define LED_PIN 5
#define TIMER_INTERVAL (1 * US_PER_SEC)

static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;
static msg_t msg_queue[8];

void led_toggle(void) {
    // Simulate LED toggle logic here
    printf("LED toggled\n");
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg_send(&msg, thread_getpid());
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize event timer
    evtimer_init_msg(&evtimer);

    // Set up the event timer to trigger every second
    evtimer_event.event.offset = (timex_t){0, 0};
    evtimer_event.event.interval = (timex_t){TIMER_INTERVAL / US_PER_SEC, (TIMER_INTERVAL % US_PER_SEC) * 1000};
    evtimer_event.event.callback = timer_callback;
    evtimer_add_msg(&evtimer, &evtimer_event, NULL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        led_toggle();
    }

    return 0;
}
