#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define INTERVAL_US   (1LU * US_PER_SEC)
#define N_MSGS        (4)

static mbox_t mbox;
static msg_t mbox_queue[N_MSGS];
static evtimer_mbox_event_t event;

void event_callback(void *arg) {
    puts("Event triggered!");
}

int main(void) {
    msg_init_queue(mbox_queue, N_MSGS);
    mbox_init(&mbox, mbox_queue, N_MSGS);
    
    event.super.interval = INTERVAL_US;
    event.super.callback = event_callback;
    evtimer_add(&event);
    
    msg_t msg;

    while (1) {
        mbox_receive(&mbox, &msg);
        puts("Processing message...");
    }

    return 0;
}