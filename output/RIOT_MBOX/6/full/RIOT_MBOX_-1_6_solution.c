#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "msg.h"
#include "mbox.h"
#include "stdio.h"

/* This example demonstrates basic message box functionality in RIOT OS. */

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1;
    while (1) {
        /* Wait some time (e.g., 1 second) */
        ztimer_sleep(ZTIMER_SEC, 1);
        
        /* Then send a message */
        printf("Sender: Sending message...\n");
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        /* Wait for the next incoming message */
        printf("Receiver: Waiting for message...\n");
        msg_receive(&msg);

        /* Process the message */
        printf("Receiver: Received message with content value: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* Stack allocation for threads */
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    char receiver_stack[THREAD_STACKSIZE_DEFAULT];

    /* Start sender thread */
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  sender_thread, NULL, "sender");

    /* Start receiver thread */
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 2, 0,
                  receiver_thread, NULL, "receiver");

    return 0;
}
