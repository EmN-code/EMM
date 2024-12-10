#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <irq_arch.h>
#include <irq.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <dht_params.h>

static volatile bool irq_status = false;

static void perform_safe_operations(void) {
    unsigned state = irq_enable();
    irq_status = true;  // Mock operation to signify critical section
    irq_restore(state);
}

int main(void) {
    puts("== Interrupt Handling Example with RIOT OS on ESP32 ==");

    // Initial state check
    printf("IRQ Status before operations: %d\n", irq_status);

    // Perform operations with interrupt management
    perform_safe_operations();

    // Final state check
    printf("IRQ Status after operations: %d\n", irq_status);

    return 0;
}