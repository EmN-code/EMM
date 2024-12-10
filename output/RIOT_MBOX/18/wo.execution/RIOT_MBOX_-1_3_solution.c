#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <thread.h>
#include <event/thread.h>
#include <MQTTClient.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[8];

void *msg_handler_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, 8);
    printf("Message handler thread started\n");

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u\n", msg.type);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t msg_handler_pid;

    msg_handler_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, msg_handler_thread, NULL, "msg_handler");

    if (msg_handler_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start message handler thread\n");
        return 1;
    }

    msg_t msg;
    int interval = 2;

    while (1) {
        msg.type = (uint16_t)rand();

        if (msg_try_send(&msg, msg_handler_pid) != 1) {
            printf("Failed to send message\n");
        }

        sleep(interval);
    }

    return 0;
}