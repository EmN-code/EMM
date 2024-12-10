
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>
#include <stdatomic.h>

#define MSG_QUEUE_SIZE 10
#define TIMER_INTERVAL (1000 * 1000U) // 1 second

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_msg_t evtimer;

static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Dummy value
    mbox_put(&mbox, &msg);
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_msg(&evtimer);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, xtimer_now64() + TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);

        // Simulate some work
        xtimer_usleep(100000); // 100 ms

        // Check the atomic variable
        if (atomic_load(&atomic_var) == 0) {
            puts("Interrupts handled correctly.");
        } else {
            puts("Interrupts not handled correctly.");
        }

        // Reset the atomic variable
        atomic_store(&atomic_var, 0);

        // Re-add the timer event
        evtimer_add_msg(&evtimer, xtimer_now64() + TIMER_INTERVAL, timer_callback, NULL);
    }

    return 0;
}
