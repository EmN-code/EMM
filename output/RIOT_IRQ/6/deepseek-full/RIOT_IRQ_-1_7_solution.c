
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void event_handler(void *arg)
{
    (void)arg;
    printf("Event received!\n");
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer for mailbox events
    evtimer_init_mbox(&evtimer.timer);

    // Create an event
    evtimer_msg_event_t event;
    event.event.handler = event_handler;
    event.event.arg = NULL;

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Simulate an interrupt by adding a message to the mailbox
    msg_t msg;
    msg.content.ptr = NULL;
    mbox_put(&mbox, &msg);

    // Wait for the event to be handled
    while (atomic_load(&interrupt_flag) == 0) {
        xtimer_usleep(1000);
    }

    printf("Interrupt handling verified!\n");

    return 0;
}
