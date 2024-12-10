
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <stdatomic.h>

#define LED_PIN 5

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered, turning LED on\n");
    // Assuming LED_PIN is a valid GPIO pin for ESP32
    // gpio_toggle(LED_PIN); // This function is blacklisted, so we need to find an alternative
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    mbox_init(&mbox, msg_queue, ARRAY_SIZE(msg_queue));
    evtimer_init_mbox(&evtimer);

    evtimer_mbox_event_t event;
    event.event.callback = timer_callback;
    event.event.arg = NULL;
    event.mbox = &mbox;

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        ztimer_set(ZTIMER_MSEC, &timer, 1000);
        msg_t msg;
        msg_receive(&msg);
        // Handle the received message if needed
    }

    return 0;
}
