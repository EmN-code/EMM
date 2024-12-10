#include "nvs_flash.h"
#include "nimble/nimble_npl.h"
#include "irq_arch.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

volatile bool irq_status;

void test_interrupt_handling(void) {
    unsigned state;
    printf("Disabling interrupts...\n");
    state = irq_disable();
    irq_status = irq_is_enabled();
    if (!irq_status) {
        printf("Interrupts are disabled successfully.\n");
    } else {
        printf("Failed to disable interrupts.\n");
    }

    printf("Restoring interrupts...\n");
    irq_restore(state);
    irq_status = irq_is_enabled();
    if (irq_status) {
        printf("Interrupts are enabled successfully.\n");
    } else {
        printf("Failed to enable interrupts.\n");
    }
}

int main(void) {
    printf("Running RIOT on ESP32 with interrupt handling...\n");

    while (1) {
        test_interrupt_handling();
        sleep(1);
    }

    return 0;
}