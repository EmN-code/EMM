
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h> // Include the correct header for atomic operations

volatile atomic_t interrupt_flag = ATOMIC_INIT(0); // Atomic variable to verify interrupt handling

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1); // Set the atomic variable to 1 when interrupt occurs
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some critical section code
    xtimer_sleep(1);

    // Restore interrupts
    irq_restore(state);

    // Enable interrupts and attach the handler
    irq_enable();
    irq_attach(0, interrupt_handler, NULL); // Attach the interrupt handler to interrupt 0

    // Wait for the interrupt to occur
    while (atomic_load(&interrupt_flag) == 0) {
        xtimer_usleep(100);
    }

    printf("Interrupt occurred, flag set to: %d\n", atomic_load(&interrupt_flag));

    return 0;
}
