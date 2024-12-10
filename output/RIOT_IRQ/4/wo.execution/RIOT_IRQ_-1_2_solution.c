
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <xtensa/hal.h>
#include <xtensa/core-macros.h>

volatile bool interrupt_occurred = false;

void interrupt_handler(void *arg)
{
    (void)arg;
    interrupt_occurred = true;
}

int main(void)
{
    // Initialize the interrupt system
    xt_set_interrupt_handler(1, interrupt_handler, NULL);
    xt_ints_on(1 << 1);

    // Test disabling and restoring interrupts
    uint32_t key = XT_RSIL(15); // Disable interrupts
    interrupt_occurred = false;
    XT_NOP(); // Simulate some work
    if (interrupt_occurred) {
        printf("Interrupt occurred while interrupts were disabled!\n");
    } else {
        printf("No interrupt occurred while interrupts were disabled.\n");
    }
    XT_RFI(key); // Restore interrupts

    // Wait for an interrupt to occur
    while (!interrupt_occurred) {
        XT_NOP();
    }

    printf("Interrupt occurred after restoring interrupts.\n");

    return 0;
}
