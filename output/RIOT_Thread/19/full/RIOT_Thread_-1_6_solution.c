#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <stdio.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD1_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIORITY (THREAD_PRIORITY_MAIN - 2)

static char thread1_stack[STACKSIZE];
static char thread2_stack[STACKSIZE];

static msg_t msg_queue[8];

void *thread1(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;

    while (1) {
        xtimer_sleep(1);
        printf("Thread 1 sending message: %lu\n", (uint32_t)msg.content.value);
        msg_send(&msg, thread_getpid_of((thread_t*)&thread2_stack));
    }
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, 8);
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Thread 2 received message with value: %lu\n", (uint32_t)msg.content.value);
    }
    return NULL;
}

int main(void) {
    thread_create(thread1_stack, sizeof(thread1_stack), THREAD1_PRIORITY, 0,
                  thread1, NULL, "thread1");
    thread_create(thread2_stack, sizeof(thread2_stack), THREAD2_PRIORITY, 0,
                  thread2, NULL, "thread2");

    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}