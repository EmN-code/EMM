
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    unsigned state;

    // Initialize the interrupt flag
    atomic_store(&interrupt_flag, 0);

    // Disable interrupts
    state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(ESP32_IRQ_NUM, interrupt_handler);

    // Enable the interrupt
    irq_enable(ESP32_IRQ_NUM);

    // Simulate some work
    xtimer_sleep(1);

    // Restore interrupts
    irq_restore(state);

    // Check if the interrupt was handled
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt was successfully handled and interrupts were restored.\n");
    } else {
        printf("Interrupt was not handled or interrupts were not restored correctly.\n");
    }

    return 0;
}
