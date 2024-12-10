#include <stdio.h>
#include <irq_arch.h>

volatile unsigned int test_var = 0;

void irq_handler_example(void) {
    // Example interrupt handler function
    printf("Interrupt triggered!\n");
    // Perform atomic operations
    __disable_irq();
    test_var++;
    __enable_irq();
}

int main() {
    printf("Starting program.\n");

    // Simulating an interrupt
    irq_handler_example();

    printf("Test variable after interrupt: %u\n", test_var);

    return 0;
}