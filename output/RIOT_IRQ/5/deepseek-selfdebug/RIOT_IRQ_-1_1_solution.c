
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
    irq_set_handler(0, interrupt_handler);
    irq_enable();

    // Test the interrupt handling
    printf("Disabling interrupts...\n");
    unsigned state = irq_disable();
    printf("Interrupts disabled. Checking interrupt flag...\n");
    if (atomic_load(&interrupt_flag) == 0) {
        printf("Interrupt flag is still 0, as expected.\n");
    } else {
        printf("Error: Interrupt flag is set unexpectedly.\n");
    }

    // Simulate an interrupt
    irq_restore(state);
    printf("Restoring interrupts. Simulating an interrupt...\n");
    irq_set(0); // Trigger the interrupt

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check the interrupt flag
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt flag is set, interrupt was handled correctly.\n");
    } else {
        printf("Error: Interrupt flag is not set, interrupt was not handled.\n");
    }

    return 0;
}
