
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_store(&atomic_var, 0);

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some critical section code
    printf("Critical section: Interrupts are disabled\n");

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt handler
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST,
                  interrupt_handler, NULL, "interrupt_handler");

    // Wait for the interrupt to be handled
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    printf("Interrupt handling test completed.\n");

    return 0;
}
