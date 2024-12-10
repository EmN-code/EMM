#include <stdio.h>
#include <stdint.h>
#include <string.h>
// Include the RIOT kernel and synchronization library headers
#include "kernel.h"
#include "msg.h"

// Define a message box and its capacity
#define MBOX_SIZE 8
static msg_t msg_queue[MBOX_SIZE];
static mbox_t mbox;

// Thread stack sizes and priorities
char thread_stack[THREAD_STACKSIZE_MAIN];

// Function to process messages
void *msg_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message: %lu\n", msg.content.value);
    }
    return NULL;
}

// Function to periodically send messages
void send_messages(void) {
    msg_t msg;
    for (uint32_t i = 0; i < 10; ++i) {
        msg.content.value = i;
        mbox_put(&mbox, &msg);
        xtimer_sleep(1);
    }
}

int main(void) {
    // Initialize message box
    mbox_init(&mbox, msg_queue, MBOX_SIZE);

    // Create a thread for message handling
    kernel_pid_t msg_thread = thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, msg_handler, NULL, "msg_handler");
    
    if (msg_thread <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    // Start sending messages periodically
    send_messages();

    return 0;
}