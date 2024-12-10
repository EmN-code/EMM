#include <thread_arch.h>
#include <thread_config.h>
#include <kernel_defines.h>
#include <stdio.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN  (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_MAIN_ALT   (THREAD_PRIORITY_MAIN - 2)

char thread_stack1[THREAD_STACKSIZE_MAIN];
char thread_stack2[THREAD_STACKSIZE_MAIN];

void *thread1_entry(void *arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread2_entry(void *arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    /* Create thread 1 */
    thread1_pid = thread_create(thread_stack1, sizeof(thread_stack1),
                                THREAD_PRIORITY_MAIN, 0,
                                thread1_entry, NULL, "thread1");
    
    /* Create thread 2 */
    thread2_pid = thread_create(thread_stack2, sizeof(thread_stack2),
                                THREAD_PRIORITY_MAIN_ALT, 0,
                                thread2_entry, NULL, "thread2");

    /* Run threads */
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN_ALT);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN);

    return 0;
}