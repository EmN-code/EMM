
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"

#define STACKSIZE 2048

volatile uint32_t atomic_var = 0;

void task_func(void *arg) {
    (void)arg;
    while (1) {
        atomic_var++;
        printf("Atomic variable incremented to: %lu\n", (unsigned long)atomic_var);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
}

int main(void) {
    char *stack = malloc(STACKSIZE);
    if (stack == NULL) {
        printf("Failed to allocate stack\n");
        return 1;
    }

    kernel_pid_t pid = thread_create(stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, task_func, NULL, "task");
    if (pid <= 0) {
        printf("Failed to create thread\n");
        free(stack);
        return 1;
    }

    while (1) {
        unsigned state = irq_disable();
        uint32_t temp = atomic_var;
        irq_restore(state);

        printf("Atomic variable read as: %lu\n", (unsigned long)temp);
        ztimer_sleep(ZTIMER_MSEC, 2000);
    }

    free(stack);
    return 0;
}
