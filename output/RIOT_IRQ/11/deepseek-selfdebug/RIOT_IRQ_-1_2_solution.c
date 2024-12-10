
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate an interrupt by calling the handler directly
    interrupt_handler();

    // Check if the flag was set
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly while interrupts were disabled.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    // Restore interrupts to their previous state
    irq_restore(state);

    printf("Interrupt handling test completed.\n");

    return 0;
}
