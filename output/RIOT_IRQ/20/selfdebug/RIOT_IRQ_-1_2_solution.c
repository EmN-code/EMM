#include "xtimer.h"
#include "thread.h"
#include "irq_arch.h"

static uint32_t atomic_var = 0;

void test_interrupt_handling(void) {
    uint32_t state = irq_disable();
    
    // Simulate some critical section operation
    atomic_var = 1;

    // Restore interrupts
    irq_restore(state);
}

int main(void) {
    // Call the function to test interrupt handling
    test_interrupt_handling();

    // Busy loop to prevent the program from exiting
    while(1) {
        xtimer_sleep(1);
    }

    return 0;
}