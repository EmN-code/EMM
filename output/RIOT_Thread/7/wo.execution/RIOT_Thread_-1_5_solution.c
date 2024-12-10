#include <ztimer.h>  // Required for timer operations
#include <unistd.h>   // General purpose utilities
#include <timex.h>    // Time unit utilities
#include <stdlib.h>   // Standard library definitions
#include <stddef.h>   // Standard definitions
#include <stdatomic.h>// Support for atomic operations
#include <stdbool.h>  // Boolean type definitions
#include <stdio.h>    // Standard input and output operations
#include <msg.h>      // Message API
#include <mma8x5x_params.h> // Sensor parameter definitions
#include <mma8x5x_regs.h>   // Sensor register definitions
#include <mma8x5x.h> // Sensor API
#include <mbox.h>    // Mailbox functions

#define THREAD_STACKSIZE (1024)
#define THREAD_PRIORITY_SENDER (3)
#define THREAD_PRIORITY_RECEIVER (2)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static msg_t msg_queue[8];  // Message queue

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;

    for (uint8_t i = 0; i < 10; i++) {
        msg.type = i;
        printf("[Sender] Sending message with type: %d\n", msg.type);
        msg_send(&msg, thread_get_receiver());
        ztimer_sleep(ZTIMER_SEC, 1);  // Sleep for 1 second
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("[Receiver] Received message with type: %d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                             THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                                             sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    if (sender_pid <= KERNEL_PID_UNDEF || receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create threads\n");
        return 1;
    }

    msg_init_queue(msg_queue, 8);

    return 0;
}