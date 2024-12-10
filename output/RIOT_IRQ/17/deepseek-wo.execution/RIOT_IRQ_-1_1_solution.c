
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

    // Test the interrupt handling
    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();

    // Check if interrupts are disabled
    if (irq_is_in()) {
        printf("Interrupts are disabled.\n");
    } else {
        printf("Interrupts are still enabled!\n");
    }

    // Simulate some work
    xtimer_sleep(2);

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored.\n");

    // Wait for the interrupt to be triggered
    while (atomic_load(&interrupt_flag) == 0) {
        xtimer_usleep(100);
    }

    printf("Interrupt was successfully handled.\n");

    return 0;
}
