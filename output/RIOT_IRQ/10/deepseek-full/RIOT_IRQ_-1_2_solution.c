
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define ATOMIC_FLAG_INIT 0

volatile int atomic_flag = ATOMIC_FLAG_INIT;

void isr_handler(void)
{
    if (atomic_flag == 0) {
        atomic_flag = 1;
    }
}

int main(void)
{
    unsigned state;

    // Disable interrupts and save the current state
    state = irq_disable();

    // Simulate an interrupt by calling the ISR handler
    isr_handler();

    // Restore the interrupt state
    irq_restore(state);

    // Verify if the atomic flag was set correctly
    if (atomic_flag == 1) {
        puts("Interrupt handling and atomic flag update successful.");
    } else {
        puts("Interrupt handling failed.");
    }

    return 0;
}
