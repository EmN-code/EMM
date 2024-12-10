#include <thread.h>
#include <irq_arch.h>

// Explanation:
// To manage interrupt handling in RIOT-OS, we utilize functions from the irq_arch.h header, such as irq_disable and irq_restore.
// We will use a simple atomic variable to demonstrate the correctness of enabling and disabling interrupts using these functions.
// The code sets up a thread to periodically disable and restore interrupts, checking the state of the atomic variable.

atomic_t interrupt_flag = 0;

void* irq_test_thread(void* arg) {
    (void)arg; // unused parameter
    unsigned state;

    while (1) {
        // Disable interrupts and set the flag
        state = irq_disable();
        atomic_store(&interrupt_flag, 1);

        // Restore interrupts
        irq_restore(state);
        atomic_store(&interrupt_flag, 0);

        thread_sleep_s(1); // Sleep for 1 second
    }
    return NULL;
}

int main(void) {
    // Start a new thread for testing interrupt handling
    char irq_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(irq_stack, sizeof(irq_stack), THREAD_PRIORITY_MAIN - 1,
                  0, irq_test_thread, NULL, "irq_test");
    
    while (1) {
        // Main thread can perform other tasks or monitor irq_test_thread
        if (atomic_load(&interrupt_flag)) {
            puts("Interrupts temporarily disabled.");
        } else {
            puts("Interrupts enabled.");
        }
        thread_sleep_s(2); // Sleep for 2 seconds before checking again
    }

    return 0;
}