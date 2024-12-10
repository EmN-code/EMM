#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "pthread.h"
#include "event/thread.h"
#include "time_units.h"

#define INTERVAL (1U * US_PER_SEC)
#define QUEUE_SIZE 8

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[QUEUE_SIZE];

void *event_task(void *arg) {
    (void)arg;
    msg_t msg;
    msg_queue_t queue;
    msg_init_queue(&queue, QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        // Process the message (e.g., logging or acting on event)
    }
    return NULL;
}

int main(void) {
    kernel_pid_t event_pid;
    evtimer_msg_event_t event = { .msg = { .type = 0, .content.value = 0 } };
    evtimer_msg_t evtimer;

    // Initialize event thread
    event_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, event_task, NULL, "event");

    evtimer_msg_init(&evtimer);
    evtimer_msg_add_event(&evtimer, &event, INTERVAL);

    while (1) {
        evtimer_msg_run(&evtimer);
    }

    return 0;
}
