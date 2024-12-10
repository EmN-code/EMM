
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t pid;
static atomic_int_t atomic_var = ATOMIC_INIT(0);

void *interrupt_handler(void *arg)
{
    (void)arg;

    while (1) {
        // Simulate an interrupt by waiting for a short period
        xtimer_usleep(100000); // 100 ms

        // Disable interrupts
        unsigned state = irq_disable();

        // Modify the atomic variable
        atomic_store(&atomic_var, 1);

        // Restore interrupts
        irq_restore(state);

        // Wait for a short period to simulate processing time
        xtimer_usleep(100000); // 100 ms
    }

    return NULL;
}

int main(void)
{
    // Create a thread for the interrupt handler
    pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        interrupt_handler,
        NULL,
        "interrupt_handler"
    );

    while (1) {
        // Disable interrupts
        unsigned state = irq_disable();

        // Check the atomic variable
        if (atomic_load(&atomic_var) == 1) {
            printf("Interrupt was handled correctly.\n");
            atomic_store(&atomic_var, 0); // Reset the atomic variable
        }

        // Restore interrupts
        irq_restore(state);

        // Wait for a short period before checking again
        xtimer_usleep(500000); // 500 ms
    }

    return 0;
}
