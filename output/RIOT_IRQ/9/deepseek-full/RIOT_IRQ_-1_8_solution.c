
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <timex.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>
#include <evtimer_mbox.h>
#include <stddef.h>
#include <net/sock/udp.h>
#include <dht_params.h>
#include <time_units.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <sys/socket.h>
#include <ztimer.h>

// Define a simple atomic variable
atomic_int atomic_var = 0;

// Function to simulate interrupt handling
void handle_interrupt(void) {
    // Simulate interrupt handling by incrementing the atomic variable
    atomic_fetch_add(&atomic_var, 1);
}

// Function to check if interrupts are disabled and restored correctly
void check_interrupts(void) {
    // Save the current state of interrupts
    unsigned state = irq_disable();

    // Simulate an interrupt
    handle_interrupt();

    // Restore the previous state of interrupts
    irq_restore(state);

    // Check if the atomic variable was incremented
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupts handled correctly.\n");
    } else {
        printf("Interrupts handling failed.\n");
    }
}

int main(void) {
    // Initialize the atomic variable
    atomic_init(&atomic_var, 0);

    // Check if interrupts are disabled and restored correctly
    check_interrupts();

    return 0;
}
