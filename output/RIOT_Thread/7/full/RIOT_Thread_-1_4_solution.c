#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "msg.h"
#include "thread.h"
#include "kernel_defines.h"

#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE                (THREAD_STACKSIZE_DEFAULT)

static char sender_thread_stack[STACKSIZE];
static char receiver_thread_stack[STACKSIZE];

msg_t msg_queue[8]; // Message queue, size must be a power of two

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    printf("Receiver: Waiting for message...\n");

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with type %u and value %lu\n", msg.type, msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;

    for (unsigned i = 0; i < 10; ++i) {
        msg.type = 0x1234;
        msg.content.value = i;
        printf("Sender: Sending message %u to PID %d\n", i, receiver_pid);
        msg_send(&msg, receiver_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid =
        thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), RECEIVER_THREAD_PRIORITY, 0, receiver_thread, NULL, "receiver");

    if (receiver_pid == -1) {
        printf("Error: Unable to create receiver thread\n");
        return 1;
    }

    thread_create(sender_thread_stack, sizeof(sender_thread_stack), SENDER_THREAD_PRIORITY, 0, sender_thread, &receiver_pid, "sender");

    return 0;
}