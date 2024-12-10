#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <random.h>
#include <thread.h>

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE      (THREAD_STACKSIZE_MAIN)

char sender_stack[THREAD_STACKSIZE];
char receiver_stack[THREAD_STACKSIZE];

static kernel_pid_t receiver_pid;
static msg_t msg_queue[8]; // Message queue

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    while(1) {
        random_bytes(&msg.content.value, sizeof(msg.content.value));
        msg.type = 1;

        if (msg_send(&msg, receiver_pid) == 1) {
            printf("Message sent: %lu\n", msg.content.value);
        }
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Message received with value: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create receiver thread\n");
        return 1;
    }

    thread_create(sender_stack, sizeof(sender_stack), SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    // Main thread continues its execution
    return 0;
}