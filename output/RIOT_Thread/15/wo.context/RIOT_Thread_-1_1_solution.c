#include <thread.h>
#include <xtimer.h>
#include <stdio.h>

#define HIGH_PRIO_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define LOW_PRIO_THREAD_PRIORITY  (THREAD_PRIORITY_MAIN + 1)
#define THREAD_STACKSIZE_MAIN     (THREAD_STACKSIZE_DEFAULT)

static char high_prio_stack[THREAD_STACKSIZE_MAIN];
static char low_prio_stack[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[8];

void *high_prio_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        int res = msg_receive(&msg);
        if (res < 0) {
            printf("Message receive failed\n");
            continue;
        }
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *low_prio_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0;
    msg.content.value = 42;
    while(1) {
        int res = msg_send(&msg, thread_getpid());
        if (res < 1) {
            printf("Message send failed\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Low priority thread sent message\n");
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    thread_create(high_prio_stack, sizeof(high_prio_stack), HIGH_PRIO_THREAD_PRIORITY, 0, high_prio_thread, NULL, "high_prio");
    thread_create(low_prio_stack, sizeof(low_prio_stack), LOW_PRIO_THREAD_PRIORITY, 0, low_prio_thread, NULL, "low_prio");
    return 0;
}