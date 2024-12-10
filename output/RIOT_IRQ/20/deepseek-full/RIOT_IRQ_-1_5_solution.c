
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

// Define a custom interrupt handler
void custom_interrupt_handler(void) {
    printf("Interrupt handled!\n");
}

int main(void) {
    // Initialize atomic variable to track interrupt state
    atomic_int interrupt_flag = ATOMIC_VAR_INIT(0);

    // Disable interrupts
    unsigned state = irq_disable();

    // Set custom interrupt handler
    irq_set_handler(UART_IRQ_NUM, custom_interrupt_handler);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    printf("Simulating work...\n");
    xtimer_sleep(2);

    // Restore interrupt state
    irq_restore(state);

    // Check if the interrupt flag is set
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt was triggered during work.\n");
    } else {
        printf("No interrupt was triggered during work.\n");
    }

    return 0;
}
