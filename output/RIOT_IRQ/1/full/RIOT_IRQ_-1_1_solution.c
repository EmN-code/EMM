#include <nimble_scanlist.h>
#include <nimble_scanner.h>
#include <irq_arch.h>

#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>

atomic_bool irq_status;

void enable_irq(void) {
    irq_status = true;
    printf("Interrupt enabled.\n");
}

void disable_irq(void) {
    irq_status = false;
    printf("Interrupt disabled.\n");
}

bool check_irq(void) {
    return irq_status;
}

int main(void) {
    // Simulating the control of interrupts using atomic variables.
    printf("Starting interrupt handling simulation...\n");

    // Simulate enabling interrupt
    enable_irq();
    if (check_irq()) {
        printf("IRQ is enabled.\n");
    } else {
        printf("IRQ is not enabled.\n");
    }

    // Simulating disabling interrupt
    disable_irq();
    if (!check_irq()) {
        printf("IRQ is disabled.\n");
    } else {
        printf("IRQ is not disabled.\n");
    }

    // Restore interrupt
    enable_irq();
    if (check_irq()) {
        printf("IRQ has been restored.\n");
    } else {
        printf("IRQ hasn't been restored.\n");
    }

    return 0;
}