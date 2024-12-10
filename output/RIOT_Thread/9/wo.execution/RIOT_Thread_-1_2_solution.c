#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <msg.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

// Stack size for the threads
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

// Message types
#define MSG_TYPE1 (1)
#define MSG_TYPE2 (2)

static char thread_stack1[STACK_SIZE];
static char thread_stack2[STACK_SIZE];

// Message queues
static msg_t msg_queue1[8];  // Must be a power of 2
static msg_t msg_queue2[8];

void *thread_func1(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 100;
    msg.type = MSG_TYPE1;
    while (1) {
        printf("[Thread 1] Sending message with value: %lu\n", msg.content.value);
        msg_send(&msg, thread_getpid());
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    msg_t msg;
    msg_init_queue(msg_queue2, 8);
    while (1) {
        if (msg_receive(&msg) > 0) {
            printf("[Thread 2] Received message with value: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue1, 8);

    thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN - 1,
                  CREATE_STACKTEST, thread_func1, NULL, "thread1");

    thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 2,
                  CREATE_STACKTEST, thread_func2, NULL, "thread2");

    return 0;
}