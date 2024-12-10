#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "mma8x5x_regs.h"
#include <stdio.h>   // Added for printf

// Ensure that THREAD_STACKSIZE_MAIN and THREAD_PRIORITY_MAIN are defined once
#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY  (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];

void *receiver_thread(void *arg) {
    msg_t msg;
    (void)arg; // to suppress unused variable warning

    printf("Receiver thread started\n");

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u, value: %u\n", msg.type, msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_t message;
    msg_init_queue(&message, 1);

    kernel_pid_t receiver_pid = thread_create(stack, STACKSIZE, PRIORITY, 0,
                                               receiver_thread, NULL, "Receiver");

    printf("Main thread sending messages\n");

    for (uint16_t i = 0; i < 10; i++) {
        message.type = i;
        message.content.value = i * 10;

        msg_send_receive(&message, &message, receiver_pid);
    }

    return 0;
}