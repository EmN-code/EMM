// Code solution for RIOT on ESP32 for inter-thread communication using msg_t.

// Required imports in descending alphabetical order
#include <unisted.h>
#include <time_units.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>

#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE          (THREAD_STACKSIZE_MAIN)

// Define message types
#define MSG_TYPE_1 (0xAB)
#define MSG_TYPE_2 (0xCD)

msg_t msg_queue[4];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    kernel_pid_t receiver_pid = thread_getpid_of(recv_thread);

    // Send messages with different priorities
    for(int i = 0; i < 10; i++) {
        msg.type = MSG_TYPE_1;
        msg.content.value = i;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);

        msg.type = MSG_TYPE_2;
        msg.content.value = i + 100;
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    // Initialize the message queue
    msg_init_queue(msg_queue, 4);

    // Receive messages
    while (1) {
        msg_receive(&msg);
        printf("Received msg: Type=%d, Value=%d\n", msg.type, msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Thread definitions
    char sender_stack[STACKSIZE];
    char receiver_stack[STACKSIZE];

    // Create the sender thread
    if (thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO, 0,
                      sender_thread, NULL, "sender") <= KERNEL_PID_UNDEF) {
        printf("Error creating sender thread!");
        return 1;
    }

    // Create the receiver thread
    if (thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO, 0,
                      receiver_thread, NULL, "receiver") <= KERNEL_PID_UNDEF) {
        printf("Error creating receiver thread!");
        return 1;
    }

    return 0;
}