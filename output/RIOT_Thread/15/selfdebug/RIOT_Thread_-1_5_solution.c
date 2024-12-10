#include <stdio.h>
#include <thread.h>
#include <msg.h>

#define THREAD_PRIO_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIO_LOW (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE 1024

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[8];

void *high_prio_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received msg: %lu\n", msg.content.value);
    }

    return NULL;
}

void *low_prio_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = (uint32_t)arg;
        msg_send(&msg, thread_getpid());
        printf("Low priority thread sent msg: %lu\n", msg.content.value);
        xtimer_sleep(2);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    
    thread_create(stack_high, STACKSIZE, THREAD_PRIO_HIGH, 0, high_prio_thread, NULL, "high");
    thread_create(stack_low, STACKSIZE, THREAD_PRIO_LOW, 0, low_prio_thread, (void *)42, "low");

    return 0;
}