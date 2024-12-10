
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <timex.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <unistd.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <thread.h>
#include <irq.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void interrupt_handler(void *arg)
{
    (void)arg;
    unsigned state = irq_disable();
    printf("Interrupt handled!\n");
    irq_restore(state);
}

static void _evtimer_mbox_handler(event_t *event)
{
    msg_t msg;
    msg.content.ptr = event;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    xtimer_sleep(1); // Simulate some delay before enabling interrupts

    unsigned state = irq_disable();
    printf("Interrupts disabled.\n");

    // Add an event to the event timer
    event_t event;
    event.handler = interrupt_handler;
    event.arg = NULL;
    evtimer_add_msg(&evtimer, &event, 0);

    irq_restore(state);
    printf("Interrupts restored.\n");

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        event_t *event = (event_t *)msg.content.ptr;
        event->handler(event->arg);
    }

    return 0;
}
