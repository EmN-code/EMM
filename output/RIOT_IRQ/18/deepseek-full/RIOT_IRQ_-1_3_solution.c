
#include "event/thread.h"
#include "irq.h"
#include "thread.h"
#include "MQTTClient.h"

#include <stdio.h>
#include <stdbool.h>

volatile bool interrupt_flag = false;

void interrupt_handler(void)
{
    interrupt_flag = true;
}

int main(void)
{
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Restore the interrupt state
    irq_restore(state);

    // Simulate an interrupt by setting the flag
    interrupt_flag = true;

    // Wait for the interrupt to be handled
    while (!interrupt_flag) {
        thread_sleep();
    }

    // Check the interrupt flag
    if (interrupt_flag) {
        printf("Interrupt handled successfully!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
