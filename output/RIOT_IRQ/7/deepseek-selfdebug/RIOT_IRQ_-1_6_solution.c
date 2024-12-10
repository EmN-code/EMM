
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

volatile atomic_int_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    puts("Interrupt handling test with atomic variables");

    // Register an interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    atomic_store(&interrupt_flag, 0);

    // Restore interrupts
    irq_restore(state);

    // Simulate an interrupt
    irq_set(IRQ_NUM);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled
    if (atomic_load(&interrupt_flag) == 1) {
        puts("Interrupt was handled correctly");
    } else {
        puts("Interrupt was not handled");
    }

    return 0;
}
