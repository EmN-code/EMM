
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <timex.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <xtimer.h>
#include <unistd.h>
#include <dht_params.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <msg.h>
#include <thread.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate interrupt handling
void interrupt_handler(void)
{
    printf("Interrupt triggered!\n");
    atomic_fetch_add(&atomic_var, 1); // Increment atomic variable
}

// Function to check atomic variable
void check_atomic_variable(void)
{
    if (atomic_load(&atomic_var) > 0) {
        printf("Atomic variable incremented successfully!\n");
    } else {
        printf("Atomic variable not incremented!\n");
    }
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Simulate an interrupt
    interrupt_handler();

    // Check the atomic variable
    check_atomic_variable();

    printf("Interrupt handling test completed.\n");

    return 0;
}
