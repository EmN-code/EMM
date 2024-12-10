
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static char sender_stack[STACK_SIZE];
static char receiver_stack[STACK_SIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

static void *sender_thread(void *arg)
{
    (void)arg;

    msg_t msg;
    msg.content.value = 42;

    while (1) {
        msg_send(&msg, receiver_pid);
        xtimer_sleep(1);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    sender_pid = thread_create(sender_stack, sizeof(sender_stack), SENDER_THREAD_PRIORITY,
                               THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_THREAD_PRIORITY,
                                 THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
