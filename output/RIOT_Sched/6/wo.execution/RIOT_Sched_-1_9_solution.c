#include <ztimer.h>
#include <xtimer.h>
#include <thread_arch.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN  (2 * THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN + 1)

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

static void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        puts("Hello from thread!");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1),
                                       THREAD_PRIORITY_MAIN + 1, 0,
                                       thread_handler, NULL, "thread1");

    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2),
                                       THREAD_PRIORITY_MAIN + 2, 0,
                                       thread_handler, NULL, "thread2");

    /* After some arbitrary time, simulate changing priority */
    ztimer_sleep(ZTIMER_MSEC, 5000);

    printf("Changing priority of thread2\n");
    /* This part of the code will simulate a priority change by recreation */
    thread_create(stack2, sizeof(stack2),
                  THREAD_PRIORITY_MAIN + 1, 0,
                  thread_handler, NULL, "thread2_new");

    /* Now the new thread runs with the updated priority */
    return 0;
}
