#include "xtimer.h"
#include "thread.h"
#include "sched.h"
#include <stdio.h>

#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN)
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)

static char stack_one[THREAD_STACKSIZE_MAIN];
static char stack_two[THREAD_STACKSIZE_MAIN];

static msg_t queue_one[8];
static msg_t queue_two[8];

static kernel_pid_t pid_one;
static kernel_pid_t pid_two;

static void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;
    msg_send(&msg, pid_two);
    printf("Thread One sent message\n");
    return NULL;
}

static void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(queue_two, 8);
    if (msg_receive(&msg) == 1) {
        printf("Thread Two received a message with value: %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");
    
    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}