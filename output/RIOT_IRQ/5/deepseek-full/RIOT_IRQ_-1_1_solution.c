
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);

        // Disable interrupts
        unsigned state = irq_disable();

        // Check the interrupt flag
        if (atomic_load(&interrupt_flag) == 1) {
            printf("Interrupt occurred while interrupts were disabled!\n");
        } else {
            printf("No interrupt occurred while interrupts were disabled.\n");
        }

        // Restore interrupts
        irq_restore(state);

        // Clear the interrupt flag
        atomic_store(&interrupt_flag, 0);
    }

    // Disable the interrupt
    irq_disable(IRQ_NUM);

    return 0;
}
