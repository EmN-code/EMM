#include "xtimer.h"
#include "msg.h"
#include "thread.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

char thread_one_stack[THREAD_STACKSIZE];
char thread_two_stack[THREAD_STACKSIZE];

kernel_pid_t thread_one_pid;
kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Arbitrary message content
    while (1) {
        msg_send(&msg, thread_two_pid);
        printf("Message sent to thread_two\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    // Thread creation
    thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                   THREAD_PRIORITY_MAIN - 1, 0,
                                   thread_one, NULL, "thread_one");
                                   
    thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                   THREAD_PRIORITY_MAIN - 2, 0,
                                   thread_two, NULL, "thread_two");

    while (1) {
        // Main thread code (potentially doing other tasks)
        xtimer_sleep(5);
    }
    return 0;
}