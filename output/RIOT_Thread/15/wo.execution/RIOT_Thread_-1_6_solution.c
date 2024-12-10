#include <ztimer.h>
#include <stdio.h>
#include <dht.h>
#include <stdbool.h>
#include <msg.h>
#include <thread.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <event/thread.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 2)
#define MSG_QUEUE_SIZE           (8)

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *receiver_thread(void *arg) {
    (void)arg; // Unused
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }
    return NULL;
}

void *sender_thread(void *arg) {
    (void)arg; // Unused
    msg_t msg;
    msg.type = 0x1234;
    for (uint32_t i = 0; i < 5; i++) { // Sample iteration count
        msg.content.value = i;
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    thread_create(receiver_stack, sizeof(receiver_stack),
                  THREAD_PRIORITY_MAIN, 0,
                  receiver_thread, NULL, "receiver");

    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0,
                  sender_thread, NULL, "sender");

    return 0;
}