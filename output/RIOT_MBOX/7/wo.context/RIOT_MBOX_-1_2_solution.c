#include <xtimer.h>
#include <thread.h>
#include "msg.h"

#define SENDER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define RECEIVER_THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define SENDER_THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char sender_thread_stack[SENDER_THREAD_STACKSIZE];
static char receiver_thread_stack[RECEIVER_THREAD_STACKSIZE];
static kernel_pid_t sender_thread_pid;
static kernel_pid_t receiver_thread_pid;

static msg_t msg_queue[8];

void *sender_thread(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        msg.content.value = 42;  // Example message
        msg_send(&msg, receiver_thread_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    sender_thread_pid = thread_create(
        sender_thread_stack, sizeof(sender_thread_stack),
        SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
        sender_thread, NULL, "sender_thread");

    receiver_thread_pid = thread_create(
        receiver_thread_stack, sizeof(receiver_thread_stack),
        RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
        receiver_thread, NULL, "receiver_thread");

    return 0;
}
