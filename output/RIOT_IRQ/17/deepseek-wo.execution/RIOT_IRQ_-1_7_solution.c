
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <thread.h>
#include <msg.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate interrupt handling
void interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

// Main function
int main(void)
{
    // Initialize the message queue
    msg_t msg_array[16];
    msg_init_queue(msg_array, 16);

    // Create a thread to handle interrupts
    char stack[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)interrupt_handler, NULL, "interrupt_handler");

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations while interrupts are disabled
    for (int i = 0; i < 10; i++) {
        atomic_fetch_add(&atomic_var, 1);
    }

    // Restore interrupts
    irq_restore(state);

    // Verify the atomic variable value
    printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

    // Main loop
    while (1) {
        // Do nothing
        sleep(1);
    }

    return 0;
}
