
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>

// Define a global atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate interrupt handling
void interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

// Function to simulate a task that checks the atomic variable
void task_func(void *arg)
{
    (void)arg;
    while (1) {
        xtimer_sleep(1);
        printf("Atomic variable value: %d\n", atomic_load(&atomic_var));
    }
}

int main(void)
{
    // Create a thread to run the task function
    thread_t *task_thread = thread_create(NULL, THREAD_STACKSIZE_DEFAULT,
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          task_func, NULL, "task_thread");

    // Enable interrupts and set the handler
    irq_enable();
    irq_set_handler(ESP_INTR_NUM, interrupt_handler);

    // Main loop to trigger interrupts
    while (1) {
        xtimer_sleep(2);
        irq_trigger(ESP_INTR_NUM);
    }

    // Join the task thread (this will never be reached in this example)
    thread_join(task_thread);

    return 0;
}
