#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)
#define THREAD_STACKSIZE_DEFAULT THREAD_STACKSIZE_MAIN

char stack_high[THREAD_STACKSIZE_DEFAULT];
char stack_low[THREAD_STACKSIZE_DEFAULT];

msg_t queue[8];

void *thread_high(void *arg) {
    (void)arg;
    msg_t msg;
    msg_receive(&msg); 
    printf("High priority thread received message: %d\n", msg.content.value);
    return NULL;
}

void *thread_low(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42;
    msg_send(&msg, thread_getpid_of(thread_high));
    printf("Low priority thread sent message\n");
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH, 0, thread_high, NULL, "high_priority_thread");
    thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW, 0, thread_low, NULL, "low_priority_thread");
    return 0;
}