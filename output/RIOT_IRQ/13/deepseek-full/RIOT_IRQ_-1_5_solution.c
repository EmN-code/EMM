
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>

#define TIMER_INTERVAL (1000000U)  // 1 second

static atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

void timer_callback(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    xtimer_t timer;
    msg_t msg;
    mbox_t mbox;
    evtimer_msg_t evtimer;

    // Initialize the message box
    mbox_init(&mbox, &msg, 1);

    // Initialize the event timer
    evtimer_init_mbox(&evtimer, &mbox);

    // Set the timer to trigger the callback
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Wait for the interrupt to be triggered
    while (atomic_load(&interrupt_flag) == 0) {
        msg_receive(&msg);
    }

    // Verify that the interrupt was triggered
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt was successfully triggered and handled.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
