#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "msg.h"
#include "ztimer.h"

#define INTERVAL (1U * ZTIMER_SEC)  // 1 second interval
#define MAIN_QUEUE_SIZE (8)

static msg_t _msg_queue[MAIN_QUEUE_SIZE];

void send_event(void) {
    msg_t msg;
    msg.content.value = rand() % 100;  // Random event data
    printf("Sending event with data: %d\n", (int)msg.content.value);

    msg_send(&msg, sched_active_thread->pid);  // Send the message to the current thread
}

int main(void) {
    msg_init_queue(_msg_queue, MAIN_QUEUE_SIZE);
    printf("Initialized message queue\n");

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
        send_event();
    }

    return 0;
}