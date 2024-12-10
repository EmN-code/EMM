
#include "thread.h"
#include "sched.h"
#include "esp_system.h"

#define THREAD_STACKSIZE_DEFAULT 512

void thread_func_1(void *arg) {
    (void)arg;
    while (1) {
        // Thread 1 logic
        sched_change_priority(thread_get_active(), 1); // Change priority to 1
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    while (1) {
        // Thread 2 logic
        sched_change_priority(thread_get_active(), 2); // Change priority to 2
    }
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_DEFAULT];
    char stack_2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), 3, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), 3, 0, thread_func_2, NULL, "thread_2");

    (void)pid_1;
    (void)pid_2;

    return 0;
}
