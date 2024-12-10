#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define INTERVAL (5000000U) // 5 seconds in microseconds
#define MAIN_QUEUE_SIZE (8)

static msg_t _msg_queue[MAIN_QUEUE_SIZE];

// Thread variables
static char thread_stack[THREAD_STACKSIZE_MAIN];

void *event_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        puts("Sending message from event thread");
        msg_send(&msg, thread_getpid());
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_msg_queue, MAIN_QUEUE_SIZE);
    kernel_pid_t event_pid = thread_create(thread_stack, sizeof(thread_stack),
                                           THREAD_PRIORITY_MAIN, 0,
                                           event_thread, NULL, "event_thread");

    printf("Event thread started with PID: %d\n", event_pid);

    // Message reception loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        puts("Message received in main thread");
    }

    return 0;
}