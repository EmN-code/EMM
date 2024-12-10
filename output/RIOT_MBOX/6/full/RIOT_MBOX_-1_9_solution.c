#include "xtimer.h"
#include "event/thread.h"
#include "netinet/in.h"
#include "esp_system.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "msg.h"
#include "msg_init_queue.h"
#include "stdbool.h"
#include "stddef.h"
#include "thread.h"
#include "sys/socket.h"

#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MAX_MSG_QUEUE_SIZE (8)

static char sender_thread_stack[STACKSIZE];
static char receiver_thread_stack[STACKSIZE];

static void *sender_thread(void *arg);
static void *receiver_thread(void *arg);

int main(void) {
    // Initialize the message queue
    msg_t msg_queue[MAX_MSG_QUEUE_SIZE];
    msg_init_queue(msg_queue, MAX_MSG_QUEUE_SIZE);

    // Create sender thread
    kernel_pid_t sender_pid = thread_create(sender_thread_stack, STACKSIZE,
                                            PRIORITY, THREAD_CREATE_STACKTEST,
                                            sender_thread, NULL, "sender");

    // Create receiver thread
    kernel_pid_t receiver_pid = thread_create(receiver_thread_stack, STACKSIZE,
                                              PRIORITY, THREAD_CREATE_STACKTEST,
                                              receiver_thread, NULL, "receiver");

    printf("Sender PID: %d\nReceiver PID: %d\n", sender_pid, receiver_pid);
    return 0;
}

static void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example message

    while (1) {
        int result = msg_try_send(&msg, thread_getpid());
        if (result == 1) {
            printf("Message sent by sender thread.\n");
        } else {
            printf("Failed to send message by sender thread.\n");
        }
        xtimer_sleep(1); // Sleep for 1 second
    }
    return NULL;
}

static void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Message received by receiver thread, content: %lu\n", msg.content.value);
    }
    return NULL;
}