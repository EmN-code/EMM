# This code demonstrates inter-thread communication on an ESP32 using the RIOT operating system.
# We create two threads where one thread sends messages to the other using msg_t type.

#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE_MAIN     (THREAD_STACKSIZE_MAIN)

static char sender_stack[STACKSIZE_MAIN];
static char receiver_stack[STACKSIZE_MAIN];

static msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, 8);
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Receiver: Received message - Type: %u, Value: %lu\n", msg.type, msg.content.value);
        }
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int receiver_pid = thread_getpid_of(receiver_stack);
    for (unsigned i = 0; i < 10; ++i) {
        msg.type = 1;  // Set arbitrary message type
        msg.content.value = i;
        printf("Sender: Sending message - Type: 1, Value: %u\n", i);
        msg_send(&msg, receiver_pid);
        ztimer_sleep(ztimer_now() + 1000000);  // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                                             sender_thread, NULL, "sender");
    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                                               receiver_thread, NULL, "receiver");
    if (sender_pid <= 0 || receiver_pid <= 0) {
        puts("Error: Could not create threads");
        return 1;
    }
    return 0;
}