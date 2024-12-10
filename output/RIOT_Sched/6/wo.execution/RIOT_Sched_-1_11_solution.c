#include "ztimer.h"
#include "xtimer.h"
#include "thread_config.h"
#include "thread.h"
#include "msg.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread_handler(void *arg) {
    (void) arg;
    while (1) {
        xtimer_sleep(1);
        printf("Running thread %d\n", thread_getpid());
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1, thread2;
    uint8_t priority1 = 5;
    uint8_t priority2 = 7;

    thread1 = thread_create(stack1, sizeof(stack1), priority1, THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread1");
    thread2 = thread_create(stack2, sizeof(stack2), priority2, THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread2");

    printf("Threads created with initial priorities: %d, %d\n", priority1, priority2);

    // Change the priority of thread1 at runtime
    thread_set_priority(thread1, 3);
    printf("Priority of thread1 changed to: %d\n", thread_get_priority(thread1));

    // Prevent main from exiting
    while(1) {
        xtimer_sleep(2);
    }

    return 0;
}
