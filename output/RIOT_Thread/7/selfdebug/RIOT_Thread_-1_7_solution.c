#include "xtimer.h"
#include "msg.h"
#include "thread.h"

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)
#define STACK_SIZE_THREAD    (THREAD_STACKSIZE_DEFAULT)

// Thread stacks
char stack_high[STACK_SIZE_THREAD];
char stack_low[STACK_SIZE_THREAD];

// Message queue
msg_t msg_queue[8];

void *thread_high(void *arg) {
    (void) arg;
    msg_t msg;
    msg.type = 1;

    while (1) {
        printf("Thread HIGH sending message.\n");
        msg_send(&msg, thread_getpid_of(thread_low));
        xtimer_sleep(2);
    }

    return NULL;
}

void *thread_low(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Thread LOW received message of type: %d\n", msg.type);
    }

    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, 8);

    // Create threads with different priorities
    thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH, 0, thread_high, NULL, "thread_high");
    thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW, 0, thread_low, NULL, "thread_low");

    return 0;
}