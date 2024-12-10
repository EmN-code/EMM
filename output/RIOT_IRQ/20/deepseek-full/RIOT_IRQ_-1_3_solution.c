
#include "event/thread.h"
#include "irq.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"

volatile unsigned int interrupt_flag = 0;

void interrupt_handler(void)
{
    interrupt_flag = 1;
}

int main(void)
{
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(interrupt_handler);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    thread_sleep(1000000); // Sleep for 1 second

    // Restore the interrupt state
    irq_restore(state);

    // Check the interrupt flag
    if (interrupt_flag) {
        puts("Interrupt was handled successfully.");
    } else {
        puts("Interrupt was not handled.");
    }

    return 0;
}
