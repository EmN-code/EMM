#include <thread.h>
#include <stdio.h>

#define THREAD_PRIORITY_MAIN    (thread_get_priority(thread_get_active()))
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_CREATE_STACKTEST 0x08

char thread_stack[THREAD_STACKSIZE_MAIN];

void *thread_func(void *arg) {
    printf("Hello from the new thread!\n");
    return NULL;
}

int main(void) {
    printf("Starting RIOT application...\n");

    kernel_pid_t thread_pid = thread_create(thread_stack, THREAD_STACKSIZE_MAIN,
                              THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                              thread_func, NULL, "new_thread");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error: thread creation failed\n");
    } else {
        printf("Thread created successfully\n");
    }

    uint8_t new_priority = THREAD_PRIORITY_MAIN - 2;  // Adjust the priority as needed
    sched_change_priority(thread_get(thread_pid), new_priority);
    printf("Changed thread priority\n");

    return 0;
}
