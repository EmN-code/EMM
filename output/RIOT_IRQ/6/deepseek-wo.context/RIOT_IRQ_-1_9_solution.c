
#include <stdio.h>
#include <stdbool.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>

// Define an atomic variable
volatile bool interrupt_flag = false;

// Function to simulate an interrupt handler
void interrupt_handler(void)
{
    // Set the interrupt flag
    interrupt_flag = true;
}

// Function to test interrupt handling
void test_interrupt_handling(void)
{
    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate an interrupt
    interrupt_handler();

    // Check if the interrupt flag was set
    if (interrupt_flag) {
        printf("Interrupt flag was set while interrupts were disabled!\n");
    } else {
        printf("Interrupt flag was not set while interrupts were disabled.\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Simulate another interrupt
    interrupt_handler();

    // Check if the interrupt flag was set
    if (interrupt_flag) {
        printf("Interrupt flag was set after interrupts were restored.\n");
    } else {
        printf("Interrupt flag was not set after interrupts were restored.\n");
    }
}

int main(void)
{
    puts("Interrupt Handling Test");

    // Run the test
    test_interrupt_handling();

    return 0;
}
