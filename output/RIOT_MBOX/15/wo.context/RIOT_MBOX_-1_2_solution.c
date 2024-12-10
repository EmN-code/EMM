#include "ztimer.h"
#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)
#define SENDER_THREAD_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_THREAD_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];
static char sender_thread_stack[SENDER_THREAD_STACK_SIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACK_SIZE];

void *sender_thread(void *arg) {
    (void) arg;
    msg_t msg;
    msg.type = 1; // example message type

    while (1) {
        /* Wait for a second */
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("[Sender] Sending message\n");
        msg_send(&msg, thread_getpid());
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        /* Blocks until a message is received */
        if (msg_receive(&msg) == 1) {
            printf("[Receiver] Got message of type: %d\n", msg.type);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    thread_create(sender_thread_stack, sizeof(sender_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender_thread");
    
    thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), THREAD_PRIORITY_MAIN - 2,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver_thread");

    /* Let the main thread sleep, we could perform other tasks instead */
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}