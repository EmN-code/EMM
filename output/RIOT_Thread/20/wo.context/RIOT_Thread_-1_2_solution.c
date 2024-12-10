#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND (THREAD_PRIORITY_MAIN - 3)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char thread_stack[STACK_SIZE];
static char second_thread_stack[STACK_SIZE];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %" PRIu16 "\n", msg.type);
    }
    return NULL;
}

void *second_thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    int msg_type = (intptr_t)arg; 

    while (1) {
        msg.type = msg_type;
        msg_send(&msg, sched_active_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    thread_create(thread_stack, STACK_SIZE, THREAD_PRIORITY_MAIN, 0,
                  thread_handler, NULL, "receiver_thread");

    thread_create(second_thread_stack, STACK_SIZE, THREAD_PRIORITY_SECOND, 0,
                  second_thread_handler, (void *)(intptr_t)1, "sender_thread_1");

    printf("Threads started\n");

    return 0;
}
