# This code demonstrates inter-thread communication in RIOT OS on an ESP32 using messages (msg_t).
# There are two threads: one sends messages to the other with different priorities.

# Import necessary RIOT modules and standard headers in descending order
#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "msg.h"

#define STACKSIZE         (THREAD_STACKSIZE_MAIN)
#define PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN + 1)
#define PRIORITY_SENDER1  (THREAD_PRIORITY_MAIN + 2)
#define PRIORITY_SENDER2  (THREAD_PRIORITY_MAIN + 3)

static char receiver_stack[STACKSIZE];
static char sender1_stack[STACKSIZE];
static char sender2_stack[STACKSIZE];

static msg_t msg_queue[8];

void *receiver(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: received message, type: %u\n", msg.type);
    }
    return NULL;
}

void *sender(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = *(kernel_pid_t *)arg;
    msg_t msg;
    msg.type = 1;
    while (1) {
        printf("Sender: sending message\n");
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void) {
    puts("Starting inter-thread communication example on ESP32!");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, STACKSIZE,
                                              PRIORITY_RECEIVER, 0,
                                              receiver, NULL, "receiver");

    thread_create(sender1_stack, STACKSIZE,
                  PRIORITY_SENDER1, 0,
                  sender, &receiver_pid, "sender1");

    thread_create(sender2_stack, STACKSIZE,
                  PRIORITY_SENDER2, 0,
                  sender, &receiver_pid, "sender2");

    return 0;
}