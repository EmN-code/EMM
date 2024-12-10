
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    puts("Starting interrupt handling test...");

    // Register an interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Disable interrupts
    irq_disable(IRQ_NUM);

    // Set the interrupt flag to 0
    atomic_store(&interrupt_flag, 0);

    // Trigger the interrupt (simulated)
    irq_trigger(IRQ_NUM);

    // Wait a bit to ensure the interrupt would have been handled if not disabled
    xtimer_sleep(1);

    // Check if the interrupt was handled (should not be)
    if (atomic_load(&interrupt_flag) == 0) {
        puts("Interrupts were successfully disabled.");
    } else {
        puts("Interrupts were not disabled correctly.");
    }

    // Enable interrupts
    irq_enable(IRQ_NUM);

    // Set the interrupt flag to 0 again
    atomic_store(&interrupt_flag, 0);

    // Trigger the interrupt again
    irq_trigger(IRQ_NUM);

    // Wait a bit to ensure the interrupt is handled
    xtimer_sleep(1);

    // Check if the interrupt was handled (should be)
    if (atomic_load(&interrupt_flag) == 1) {
        puts("Interrupts were successfully restored.");
    } else {
        puts("Interrupts were not restored correctly.");
    }

    return 0;
}
