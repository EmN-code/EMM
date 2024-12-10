#include "xtimer.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "stdatomic.h"
#include "stdio.h"

#define MESSAGE_QUEUE_SIZE (8)
#define PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define PRIORITY_SECOND (THREAD_PRIORITY_MAIN - 2)

static char thread_stack_one[THREAD_STACKSIZE_MAIN];
static char thread_stack_two[THREAD_STACKSIZE_MAIN];

static msg_t msg_queue[MESSAGE_QUEUE_SIZE];

void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;
    ztimer_sleep(ZTIMER_MSEC, 1000);
    for (int i = 0; i < 5; i++) {
        msg.type = 1;
        msg.content.value = i;
        printf("Thread One sending message %d\n", i);
        msg_send(&msg, thread_getpid_of((thread_t *)arg));
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MESSAGE_QUEUE_SIZE);
    for (int i = 0; i < 5; i++) {
        msg_receive(&msg);
        printf("Thread Two received message with value: %u\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(thread_stack_two, sizeof(thread_stack_two), PRIORITY_SECOND, 0,
                  thread_two, NULL, "thread_two");
    thread_create(thread_stack_one, sizeof(thread_stack_one), PRIORITY_MAIN, 0,
                  thread_one, thread_getpid(), "thread_one");
    return 0;
}