#include <sys/types.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define RECEIVE_INTERVAL    (1000000LU)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; /* Arbitrary value */

    while (1) {
        xtimer_usleep(RECEIVE_INTERVAL);
        printf("Sending message with value %" PRIu32 "\n", msg.content.value);
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

int main(void) {
    /* Initialize the message queue */
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    /* Thread for sending messages */
    char sender_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    msg_t msg;
    while (1) {
        /* Wait for a message */
        msg_receive(&msg);
        printf("Received message with value %" PRIu32 "\n", msg.content.value);
    }
    return 0;
}