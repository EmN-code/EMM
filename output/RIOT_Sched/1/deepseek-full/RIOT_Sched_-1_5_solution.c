
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE_MAIN (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

typedef void (*thread_task_func_t)(void *arg);

typedef struct {
    char *stack;
    int stacksize;
    uint8_t priority;
    int flags;
    thread_task_func_t task_func;
    void *arg;
    const char *name;
} thread_t;

typedef uint16_t kernel_pid_t;

kernel_pid_t thread_create(char *stack, int stacksize, uint8_t priority, int flags, thread_task_func_t task_func, void *arg, const char *name);
void sched_change_priority(kernel_pid_t pid, uint8_t priority);
kernel_pid_t thread_get_active(void);

void thread_func_1(void *arg) {
    (void)arg;
    while (1) {
        // Thread 1 logic
        sleep(1);
    }
}

void thread_func_2(void *arg) {
    (void)arg;
    while (1) {
        // Thread 2 logic
        sleep(1);
    }
}

int main(void) {
    char stack_1[THREAD_STACKSIZE_MAIN];
    char stack_2[THREAD_STACKSIZE_MAIN];

    kernel_pid_t pid_1 = thread_create(stack_1, sizeof(stack_1), THREAD_PRIORITY_MAIN, 0, thread_func_1, NULL, "thread_1");
    kernel_pid_t pid_2 = thread_create(stack_2, sizeof(stack_2), THREAD_PRIORITY_MAIN, 0, thread_func_2, NULL, "thread_2");

    // Change priority of thread 1 to a higher priority
    sched_change_priority(pid_1, THREAD_PRIORITY_MAIN + 1);

    // Change priority of thread 2 to a lower priority
    sched_change_priority(pid_2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
