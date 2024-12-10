
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
    irq_disable();
    irq_set_handler(IRQ_NUM, interrupt_handler);
    irq_enable(IRQ_NUM);
    irq_restore(irq_disable());

    // Simulate an interrupt by triggering it manually
    irq_set(IRQ_NUM);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled correctly
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
