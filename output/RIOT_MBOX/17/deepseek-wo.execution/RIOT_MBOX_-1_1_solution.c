
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 10
#define SENDER_THREAD_STACK_SIZE 512
#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t sender_pid;

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example event value

    while (1) {
        msg_send(&msg, sender_pid);
        xtimer_sleep(5); // Send event every 5 seconds
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    char stack[SENDER_THREAD_STACK_SIZE];
    sender_pid = thread_create(stack, sizeof(stack), SENDER_THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
