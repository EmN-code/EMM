
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define ESP32_IRQ_NUM 1

volatile int interrupt_flag = 0;

void interrupt_handler(void)
{
    interrupt_flag = 1;
}

int main(void)
{
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(ESP32_IRQ_NUM, interrupt_handler);

    // Enable the interrupt
    irq_enable(ESP32_IRQ_NUM);

    // Simulate some work
    thread_sleep();

    // Restore interrupts
    irq_restore(state);

    // Check the interrupt flag
    if (interrupt_flag) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt was not handled.");
    }

    return 0;
}
