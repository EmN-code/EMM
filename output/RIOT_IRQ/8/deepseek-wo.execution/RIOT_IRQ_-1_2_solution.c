
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <event/thread.h>

// Define a simple atomic variable type
typedef volatile uint32_t atomic_t;

// Function to initialize atomic variable
#define ATOMIC_INIT(val) (val)

// Function to load atomic variable
static inline uint32_t atomic_load(atomic_t *var) {
    return *var;
}

// Function to store value in atomic variable
static inline void atomic_store(atomic_t *var, uint32_t val) {
    *var = val;
}

// Thread stack size
#define THREAD_STACKSIZE 512

// Thread function prototype
void *thread_func(void *arg);

// Global atomic variable
atomic_t atomic_var = ATOMIC_INIT(0);

int main(void)
{
    // Create a thread
    thread_t *thread = thread_create(
        (char *)malloc(THREAD_STACKSIZE),
        THREAD_STACKSIZE,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread_func,
        NULL,
        "worker_thread"
    );

    // Check if thread creation was successful
    if (thread == NULL) {
        printf("Failed to create thread\n");
        return 1;
    }

    // Main loop
    while (1) {
        // Disable interrupts
        unsigned state = irq_disable();

        // Modify atomic variable
        atomic_store(&atomic_var, atomic_load(&atomic_var) + 1);

        // Restore interrupts
        irq_restore(state);

        // Print atomic variable value
        printf("Main thread: atomic_var = %u\n", atomic_load(&atomic_var));

        // Sleep for a while
        xtimer_sleep(1);
    }

    return 0;
}

// Thread function
void *thread_func(void *arg)
{
    while (1) {
        // Disable interrupts
        unsigned state = irq_disable();

        // Modify atomic variable
        atomic_store(&atomic_var, atomic_load(&atomic_var) + 1);

        // Restore interrupts
        irq_restore(state);

        // Print atomic variable value
        printf("Worker thread: atomic_var = %u\n", atomic_load(&atomic_var));

        // Sleep for a while
        xtimer_sleep(2);
    }

    return NULL;
}
