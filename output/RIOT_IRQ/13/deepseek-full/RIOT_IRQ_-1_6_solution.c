
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <stdbool.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <msg.h>
#include <thread.h>
#include <mbox.h>
#include <stddef.h>
#include <ztimer.h>
#include <xtimer.h>

// Define atomic variables
atomic_int atomic_var = ATOMIC_VAR_INIT(0);
atomic_int atomic_var_during_isr = ATOMIC_VAR_INIT(0);

// Function to handle interrupts
void isr_handler(void)
{
    atomic_store(&atomic_var_during_isr, atomic_var);
    printf("Interrupt handled, atomic_var_during_isr: %d\n", atomic_var_during_isr);
}

int main(void)
{
    // Initialize the event timer for mbox events
    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    // Initialize the mbox
    msg_t msg_queue[8];
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, 8);

    // Set up the timer to trigger an interrupt
    xtimer_t timer;
    xtimer_set(&timer, 1000000); // 1 second

    // Enable interrupts
    unsigned state = irq_enable();

    // Main loop
    while (1) {
        // Receive a message (blocking)
        msg_t msg;
        msg_receive(&msg);

        // Check if the interrupt was handled correctly
        if (atomic_load(&atomic_var) == atomic_load(&atomic_var_during_isr)) {
            printf("Interrupts disabled and restored correctly.\n");
        } else {
            printf("Interrupts handling failed.\n");
        }

        // Restore the interrupt state
        irq_restore(state);
    }

    return 0;
}
