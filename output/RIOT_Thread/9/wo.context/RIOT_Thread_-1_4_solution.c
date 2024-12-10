#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static msg_t msg_queue[8];

static void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg);
    printf("High priority thread received message with type: %u\n", msg.type);
    return NULL;
}

static void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg_send(&msg, thread_getpid_of(high_priority_thread));
    printf("Low priority thread sent message with type: %u\n", msg.type);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH,
                  THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");
    thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW,
                  THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    return 0;
}
