
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <thread.h>
#include <ztimer.h>
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

    // Create a thread to increment the counter
    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST, task_func, NULL, "counter_thread");

    // Main loop to check the counter value
    while (1) {
        unsigned state = irq_disable();
        int current_counter = atomic_load(&counter);
        irq_restore(state);

        printf("Counter value: %d\n", current_counter);

        // Sleep for 2 seconds
        ztimer_sleep(ZTIMER_MSEC, 2000);
    }

    return 0;
}