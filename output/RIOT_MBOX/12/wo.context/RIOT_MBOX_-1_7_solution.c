#include <stdio.h>
#include <stdlib.h>

#include "kernel.h"
#include "msg.h"
#include "thread.h"
#include "ztimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define INTERVAL (1U * US_PER_SEC)
static char thread_stack[THREAD_STACKSIZE_MAIN];

void *_event_sender(void *arg) {
    (void)arg;
    msg_t msg;
    ztimer_t timer;

    ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL, &msg, sched_active_pid);
    
    while (1) {
        msg_receive(&msg);
        printf("Event received with value: %lu\n", (unsigned long)timer.long_now);
        ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL, &msg, sched_active_pid);
    }
    return NULL;
}

int main(void) {
    puts("RIOT message box example application.\n");
    
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _event_sender, NULL, "event sender");
    
    return 0;
}