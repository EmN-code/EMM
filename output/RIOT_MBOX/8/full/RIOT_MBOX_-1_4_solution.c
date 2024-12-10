#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <string.h>
#include <mqttclient.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

// Message structure
msg_t msg_queue[8];

// Thread function
void *thread_function(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);

    msg_t msg;
    while (1) {
        // Receive message
        msg_receive(&msg);

        // Print message content
        printf("Received message with value: %lu\n", msg.content.value);

        // Pause briefly to simulate work
        xtimer_sleep(2);

        // Send message back as a reply
        msg_reply(&msg, &msg);
    }
    return NULL;
}

int main(void) {
    // Stack allocation for the new thread
    char stack[STACKSIZE];

    // Create a thread to handle messages
    thread_create(stack, sizeof(stack), PRIORITY, 0, thread_function, NULL, "msg_handler");

    // Main loop
    while (1) {
        msg_t msg;
        msg.content.value = (uint32_t)atomic_fetch_add(&counter, 1);

        // Send message
        msg_send(&msg, thread_getpid());

        // Wait for reply
        msg_reply(&msg, &msg);

        // Log event processed
        printf("Processed message with value: %lu\n", msg.content.value);

        // Wait before next message
        xtimer_sleep(5);
    }
}