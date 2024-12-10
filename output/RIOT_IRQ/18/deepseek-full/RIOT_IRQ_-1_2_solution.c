
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define FLAG_SET 1
#define FLAG_NOT_SET 0

static volatile int interrupt_flag = FLAG_NOT_SET;

static void interrupt_handler(void *arg)
{
    (void)arg;
    interrupt_flag = FLAG_SET;
}

int main(void)
{
    unsigned state;

    // Disable interrupts and save the current state
    state = irq_disable();

    // Simulate some critical section code
    // For example, setting a flag
    interrupt_flag = FLAG_NOT_SET;

    // Restore interrupts to the previous state
    irq_restore(state);

    // Set up the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);
    irq_enable(IRQ_NUM);

    // Simulate an interrupt by triggering the IRQ
    irq_set(IRQ_NUM);

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check if the interrupt flag was set
    if (interrupt_flag == FLAG_SET) {
        printf("Interrupt was successfully handled!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
