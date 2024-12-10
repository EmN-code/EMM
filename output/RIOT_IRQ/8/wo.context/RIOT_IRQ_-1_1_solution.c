#include "irq_arch.h"
#include <stdio.h>

volatile unsigned int interrupt_flag = 0;
volatile unsigned int irq_status = 0;

void interrupt_enable(void) {
    if (!interrupt_flag) {
        irq_status = irq_disable();
        interrupt_flag = 1;
    }
}

void interrupt_restore(void) {
    if (interrupt_flag) {
        irq_restore(irq_status);
        interrupt_flag = 0;
    }
}

void test_interrupt_handling(void) {
    printf("Testing interrupt handling...\n");

    interrupt_enable();
    printf("Interrupts are disabled. Status: %d\n", irq_status);

    interrupt_restore();
    printf("Interrupts are restored.\n");
}

int main(void) {
    printf("Starting program...\n");
    test_interrupt_handling();
    return 0;
}