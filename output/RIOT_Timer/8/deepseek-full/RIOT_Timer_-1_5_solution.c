
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stdatomic.h"

#define LED_PIN 5

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

void led_toggle(void) {
    // Simulate toggling the LED
    printf("LED toggled\n");
}

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 0;
    mbox_put(&mbox, &msg);
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Set up the timer to trigger every second
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        led_toggle();
    }

    return 0;
}
