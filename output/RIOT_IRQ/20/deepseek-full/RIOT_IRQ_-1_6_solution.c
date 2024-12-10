
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dht.h>
#include <dht_params.h>
#include <time_units.h>
#include <event/thread.h>
#include <MQTTClient.h>
#include <irq.h>
#include <thread.h>

// Define a simple atomic variable
static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate some work
void simulate_work(void) {
    printf("Simulating work...\n");
    xtimer_sleep(2); // Sleep for 2 seconds to simulate work
}

// Interrupt handler function
void irq_handler(void) {
    printf("Interrupt triggered!\n");
    atomic_fetch_add(&atomic_var, 1); // Increment atomic variable
}

int main(void) {
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(UART_IRQ_NUM, irq_handler);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    simulate_work();

    // Restore interrupts to their previous state
    irq_restore(state);

    // Check the atomic variable to verify if the interrupt was handled
    if (atomic_load(&atomic_var) > 0) {
        printf("Interrupt was successfully handled!\n");
    } else {
        printf("Interrupt was not handled!\n");
    }

    return 0;
}
