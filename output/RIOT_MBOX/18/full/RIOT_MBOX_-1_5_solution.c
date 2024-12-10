#include <xtimer.h>
#include <ztimer.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer.h>
#include <evtimer_msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Define stack size and other parameters
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define MBOX_SIZE 8

// Declare a message struct and a message buffer
static char thread_stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[THREAD_STACKSIZE_MAIN];
static mbox_t mbox;

// Sender thread function
void *sender(void *arg) {
    (void)arg;
    int count = 0;
    while (1) {
        msg_t msg;
        msg.type = count;  // setting the message type as the counter value
        mbox_send(&mbox, &msg);
        printf("Sent message %d\n", count);
        count++;
        ztimer_sleep(ZTIMER_USEC, 1000000);  // 1 second delay
    }
    return NULL;
}

// Receiver thread function
void *receiver(void *arg) {
    (void)arg;
    while (1) {
        msg_t msg;
        mbox_receive(&mbox, &msg);
        printf("Received message %d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    // Initialize mailbox
    mbox_init(&mbox, msg_queue, MBOX_SIZE);

    // Create sender and receiver threads
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    thread_create(thread_stack + THREAD_STACKSIZE_DEFAULT, sizeof(thread_stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    // Adding sleep to let threads run their course (adjust the time as needed)
    while(1) {
        sleep(5);
    }

    return 0;
}