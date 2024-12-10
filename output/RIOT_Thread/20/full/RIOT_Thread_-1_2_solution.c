#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <thread.h>
#include <msg.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER   (THREAD_PRIORITY_MAIN - 2)

#define QUEUE_SIZE               (8)

static char stack_sender[THREAD_STACKSIZE_MAIN];
static char stack_receiver[THREAD_STACKSIZE_MAIN];

static msg_t queue[QUEUE_SIZE];
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void) arg;
    msg_t msg;
    msg.content.value = 42; // Some arbitrary data

    while (1) {
        printf("Sender thread: Sending message with content %lu\n", (uint32_t)msg.content.value);
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void) arg;
    msg_t msg;

    msg_init_queue(queue, QUEUE_SIZE);

    while (1) {
        if (msg_receive(&msg) < 0) {
            puts("Failed to receive message");
            continue;
        }
        printf("Receiver thread: Received message with content %lu\n", (uint32_t)msg.content.value);
    }
    return NULL;
}

int main(void) {
    puts("RIOT OS thread communication example");

    receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver), THREAD_PRIORITY_MAIN,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    thread_create(stack_sender, sizeof(stack_sender), THREAD_PRIORITY_SENDER,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}