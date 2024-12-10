
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void isr(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Attach the interrupt service routine
    irq_attach(0, isr, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_store(&flag, 0);

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to occur
    while (atomic_load(&flag) == 0) {
        thread_yield();
    }

    printf("Interrupt handled successfully!\n");

    return 0;
}
