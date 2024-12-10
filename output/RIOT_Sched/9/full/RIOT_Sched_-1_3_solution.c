#include <thread_arch.h>
#include <thread.h>
#include <sched.h>
#include <event/thread.h>
#include <MQTTClient.h>
#include <architecture.h>

#define STACKSIZE_THREAD    (KERNEL_CONF_STACKSIZE_DEFAULT)

char stack_thread1[STACKSIZE_THREAD];
char stack_thread2[STACKSIZE_THREAD];

void *thread_function(void *arg) {
    (void)arg;
    while (1) {
        // Simulate thread activity
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_thread1, pid_thread2;

    // Create two threads
    pid_thread1 = thread_create(stack_thread1, STACKSIZE_THREAD,
                                THREAD_PRIORITY_MAIN - 1, 0,
                                thread_function, NULL, "thread1");

    pid_thread2 = thread_create(stack_thread2, STACKSIZE_THREAD,
                                THREAD_PRIORITY_MAIN - 2, 0,
                                thread_function, NULL, "thread2");

    // Change the priority of thread1
    sched_change_priority(thread_get(pid_thread1), THREAD_PRIORITY_MAIN - 3);

    // Change the priority of thread2
    sched_change_priority(thread_get(pid_thread2), THREAD_PRIORITY_MAIN - 4);

    while (1) {
        thread_yield();
    }

    return 0;
}