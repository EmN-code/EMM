// Description: This code demonstrates how to set up a message box using RIOT OS on an ESP32. It continuously sends messages at set intervals to illustrate mailbox usage. The code includes necessary initializations and a loop for message sending.

#include "msg.h"
#include "kernel.h"
#include "event.h"
#include "xtimer.h"
#include "stdio.h"

// Define stack size for message handling thread
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

// Define message queue size
#define MSG_QUEUE_SIZE   (8)

// Define interval for message sending (in microseconds)
#define MSG_INTERVAL      (1000000)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char thread_stack[THREAD_STACKSIZE];

void *msg_handler(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t msg_thread_pid = thread_create(
        thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, msg_handler, NULL, "msg_handler");

    if (msg_thread_pid < 0) {
        printf("Error: unable to create message handler thread\n");
        return 1;
    }

    msg_t msg;
    msg.type = 1;

    while (1) {
        printf("Sending message with type: %d\n", msg.type);
        msg_try_send(&msg, msg_thread_pid);
        xtimer_usleep(MSG_INTERVAL);
    }
    return 0;
}