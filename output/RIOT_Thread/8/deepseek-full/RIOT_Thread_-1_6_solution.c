
#include "msg.h"
#include "thread.h"
#include "kernel_defines.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#define STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

#define NUM_MESSAGES 8

static char high_stack[STACKSIZE_DEFAULT];
static char low_stack[STACKSIZE_DEFAULT];

static kernel_pid_t high_pid;
static kernel_pid_t low_pid;

static void *high_priority_thread(void *arg)
{
    msg_t msg_queue[NUM_MESSAGES];
    msg_init_queue(msg_queue, NUM_MESSAGES);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("High priority thread received message: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

static void *low_priority_thread(void *arg)
{
    msg_t msg_queue[NUM_MESSAGES];
    msg_init_queue(msg_queue, NUM_MESSAGES);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Low priority thread received message: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;

    high_pid = thread_create(high_stack, sizeof(high_stack), PRIORITY_MAIN + 1, 0, high_priority_thread, NULL, "high_priority");
    low_pid = thread_create(low_stack, sizeof(low_stack), PRIORITY_MAIN - 1, 0, low_priority_thread, NULL, "low_priority");

    msg.content.value = 123;
    msg_send_receive(&msg, &msg, high_pid);
    printf("Main thread sent message to high priority thread: %" PRIu32 "\n", msg.content.value);

    msg.content.value = 456;
    msg_send_receive(&msg, &msg, low_pid);
    printf("Main thread sent message to low priority thread: %" PRIu32 "\n", msg.content.value);

    return 0;
}
