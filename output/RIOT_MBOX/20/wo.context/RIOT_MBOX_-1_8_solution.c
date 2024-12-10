#include "ztimer.h"
#include "thread.h"
#include "msg.h"
#include "kernel.h"

#ifndef DEBUG_EXTRA_STACKSIZE
#define DEBUG_EXTRA_STACKSIZE     (0)
#endif

#define MAIN_QUEUE_SIZE     (8)
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

#define INTERVAL_USEC       (5000000U) // 5 seconds

static char stack[THREAD_STACKSIZE_MAIN + DEBUG_EXTRA_STACKSIZE];

void *event_thread(void *arg) {
    (void)arg;
    evtimer_t evtimer;
    evtimer_init(&evtimer);
    evtimer_event_t event;
    ztimer_sleep(ZTIMER_USEC, INTERVAL_USEC);
    
    while (1) {
        evtimer_add_msg(&evtimer, &event, thread_getpid());
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, event_thread, NULL, "event_thread");
    
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received an event!\n");
    }

    return 0;
}