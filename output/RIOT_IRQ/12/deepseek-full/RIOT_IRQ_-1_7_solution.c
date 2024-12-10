
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <kernel_defines.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <net/sock/util.h>
#include <ztimer.h>

// Define an atomic variable to track interrupt status
static atomic_bool isr_flag = ATOMIC_VAR_INIT(false);

// Function to handle the interrupt
void handle_interrupt(void)
{
    // Set the interrupt flag
    atomic_store(&isr_flag, true);
}

// Main function
int main(void)
{
    // Initialize the interrupt flag
    atomic_store(&isr_flag, false);

    // Simulate an interrupt by calling the handler
    handle_interrupt();

    // Check if the interrupt was handled correctly
    if (atomic_load(&isr_flag)) {
        printf("Interrupt handled successfully!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
