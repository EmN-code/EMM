
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_int_t atomic_var;  // Correct type for atomic variable

void irq_handler(int irq_num) {
    printf("Interrupt triggered!\n");
    atomic_store(&atomic_var, 1);  // Correct function to store atomic value
}

int main(void) {
    atomic_init(&atomic_var, 0);  // Correct function to initialize atomic variable

    printf("Disabling interrupts...\n");
    irq_disable();

    printf("Setting interrupt handler...\n");
    irq_set_handler(IRQ_NUM, irq_handler);  // Correct function to set interrupt handler

    printf("Enabling interrupts...\n");
    irq_enable();

    printf("Waiting for interrupt...\n");
    while (atomic_load(&atomic_var) == 0) {  // Correct function to load atomic value
        thread_yield();
    }

    printf("Interrupt handled successfully!\n");
    return 0;
}
