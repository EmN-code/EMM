
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <timex.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <xtimer.h>
#include <ztimer.h>
#include <event/thread.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

// Define a simple atomic variable
atomic_int atomic_var = ATOMIC_VAR_INIT(0);

// Function to simulate an interrupt
void simulate_interrupt(void)
{
    printf("Interrupt simulated!\n");
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate an interrupt
    simulate_interrupt();

    // Restore interrupts
    irq_restore(state);

    // Verify the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt handling and atomic variable verification successful!\n");
    } else {
        printf("Interrupt handling or atomic variable verification failed!\n");
    }

    return 0;
}
