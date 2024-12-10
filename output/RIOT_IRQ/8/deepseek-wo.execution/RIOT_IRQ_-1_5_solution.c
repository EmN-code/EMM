
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <thread.h>
#include <irq.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate interrupt handling
void interrupt_handler(void *arg) {
    (void)arg;
    atomic_fetch_add(&atomic_var, 1);
}

// Function to test interrupt handling
void test_interrupt_handling(void) {
    unsigned state = irq_disable();
    atomic_fetch_add(&atomic_var, 10);
    irq_restore(state);
}

int main(void) {
    // Create a thread to handle interrupts
    char stack[THREAD_STACKSIZE_DEFAULT];
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_WOUT_YIELD, interrupt_handler, NULL, "interrupt_handler");

    // Test the interrupt handling mechanism
    test_interrupt_handling();

    // Print the atomic variable to verify the result
    printf("Atomic variable value: %d\n", atomic_load(&atomic_var));

    return 0;
}
