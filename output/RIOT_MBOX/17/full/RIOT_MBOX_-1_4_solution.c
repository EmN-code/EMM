#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <msg.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static mbox_t my_demo_mbox;
static char my_demo_mbox_stack[THREAD_STACKSIZE_MAIN];

static void *my_demo_mbox_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        /* Receive messages and process them */
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("RIOT Message Box Example");

    msg_t queue[8];
    msg_init_queue(queue, 8);

    /* Initialize mailbox for event timer */
    mbox_init(&my_demo_mbox, my_demo_mbox_stack, sizeof(my_demo_mbox_stack), 8);

    kernel_pid_t mbox_thread_pid = thread_create(my_demo_mbox_stack, sizeof(my_demo_mbox_stack),
                                                 THREAD_PRIORITY_MAIN - 1,
                                                 THREAD_CREATE_STACKTEST,
                                                 my_demo_mbox_thread, NULL, "mbox");

    evtimer_t evtimer;
    evtimer_msg_event_t event;

    /* Initialize and configure evtimer */
    evtimer_init_msg(&evtimer);

    /* Set up the event to send a message to our mailbox thread's PID */
    evtimer_msg_event_init(&event, &queue[0], mbox_thread_pid, NULL);

    /* Register event with evtimer to trigger every interval */
    evtimer_add_msg(&evtimer, &event, mbox_thread_pid);

    while (1) {
        xtimer_sleep(INTERVAL / US_PER_SEC);
        /* Trigger the evtimer */
        evtimer_set(&evtimer, INTERVAL);
    }

    return 0; // Will never reach
}