#include "thread.h"
#include "msg.h"
#include <stdio.h>

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW  (THREAD_PRIORITY_MAIN + 1)
#define MSG_QUEUE_SIZE       (8)

static kernel_pid_t high_prio_thread_id;
static kernel_pid_t low_prio_thread_id;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    printf("Low priority thread running.\n");

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: 0x%" PRIx32 "\n", msg.content.value);
    }
    return NULL;
}

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;
    printf("High priority thread running.\n");

    while (1) {
        msg.content.value = 0x1234;
        msg_send(&msg, low_prio_thread_id);
        printf("High priority thread sent message.\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    char high_prio_stack[THREAD_STACKSIZE];
    char low_prio_stack[THREAD_STACKSIZE];

    high_prio_thread_id = thread_create(high_prio_stack, sizeof(high_prio_stack),
                                        THREAD_PRIORITY_HIGH, 0,
                                        high_priority_thread, NULL, "high_prio_thread");

    low_prio_thread_id = thread_create(low_prio_stack, sizeof(low_prio_stack),
                                       THREAD_PRIORITY_LOW, 0,
                                       low_priority_thread, NULL, "low_prio_thread");

    return 0;
}