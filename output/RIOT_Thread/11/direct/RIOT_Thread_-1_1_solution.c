#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER_HIGH (THREAD_PRIORITY_MAIN - 2)
#define THREAD_PRIORITY_SENDER_LOW (THREAD_PRIORITY_MAIN - 3)

static char stack_receiver[THREAD_STACKSIZE_MAIN];
static char stack_sender_high[THREAD_STACKSIZE_MAIN];
static char stack_sender_low[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[8];

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) < 0) {
            printf("Error receiving message\n");
        } else {
            printf("Received message: %ld\n", (long)msg.content.value);
        }
    }
    return NULL;
}

void *sender_thread(void *arg) {
    long priority = (long)arg;
    msg_t msg;
    msg.type = 0;
    for (;;) {
        msg.content.value = priority;
        if (msg_send(&msg, thread_get(receiver_pid)) < 0) {
            printf("Error sending message\n");
        } else {
            printf("Sent message with priority %ld\n", priority);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid;
    kernel_pid_t sender_high_pid;
    kernel_pid_t sender_low_pid;

    msg_init_queue(msg_queue, 8);

    receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver),
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver_thread, NULL, "receiver");

    sender_high_pid = thread_create(stack_sender_high, sizeof(stack_sender_high),
                                    THREAD_PRIORITY_SENDER_HIGH, THREAD_CREATE_SLEEPING,
                                    sender_thread, (void *)1, "sender_high");

    sender_low_pid = thread_create(stack_sender_low, sizeof(stack_sender_low),
                                   THREAD_PRIORITY_SENDER_LOW, THREAD_CREATE_SLEEPING,
                                   sender_thread, (void *)0, "sender_low");

    thread_wakeup(sender_high_pid);
    thread_wakeup(sender_low_pid);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}