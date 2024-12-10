
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <timex.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <unistd.h>
#include <dht_params.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <msg.h>
#include <net/emcute.h>
#include <thread.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

// Define a simple atomic variable
atomic_int atomic_var = 0;

// Function to simulate an interrupt handler
void interrupt_handler(void)
{
    atomic_fetch_add(&atomic_var, 1);
}

// Main function
int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Simulate an interrupt
    interrupt_handler();

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt handled correctly. Atomic variable incremented.\n");
    } else {
        printf("Interrupt handling failed. Atomic variable not incremented.\n");
    }

    return 0;
}
