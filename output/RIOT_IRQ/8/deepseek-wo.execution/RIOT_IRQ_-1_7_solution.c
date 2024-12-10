
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <thread.h>
#include <irq.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void task_func(void *arg) {
    (void)arg;
    while (1) {
        atomic_fetch_add(&counter, 1);
        xtimer_sleep(1);
    }
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid;

    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, task_func, NULL, "counter_thread");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    while (1) {
        unsigned state = irq_disable();
        int current_counter = atomic_load(&counter);
        irq_restore(state);

        printf("Counter value: %d\n", current_counter);
        xtimer_sleep(2);
    }

    return 0;
}
