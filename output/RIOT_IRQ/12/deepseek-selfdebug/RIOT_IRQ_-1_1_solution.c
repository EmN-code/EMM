
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_set(&interrupt_flag, 1);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Restore interrupts
    irq_restore(state);

    // Simulate some work
    xtimer_sleep(1);

    // Check if the interrupt was handled
    if (atomic_get(&interrupt_flag) == 1) {
        printf("Interrupt was successfully handled and interrupts were restored correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
