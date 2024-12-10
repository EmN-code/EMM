#include <xtimer.h>
#include <thread.h>
#include <stdio.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define LOW_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define HIGH_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char low_stack[STACK_SIZE];
static char high_stack[STACK_SIZE];

msg_t msg_queue_low[8];
msg_t msg_queue_high[8];

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;

    while (1) {
        printf("Low priority thread: Sending message with value %u\n", msg.content.value);
        msg_send(&msg, thread_getpid() + 1);
        xtimer_sleep(1);
    }

    return NULL;
}

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue_high, 8);

    while (1) {
        msg_receive(&msg);
        printf("High priority thread: Received message with value %u\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue_low, 8);

    thread_create(high_stack, STACK_SIZE, HIGH_PRIORITY, 0, high_priority_thread, NULL, "high");
    thread_create(low_stack, STACK_SIZE, LOW_PRIORITY, 0, low_priority_thread, NULL, "low");

    return 0;
}