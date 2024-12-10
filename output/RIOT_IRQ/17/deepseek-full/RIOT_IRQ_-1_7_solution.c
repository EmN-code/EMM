
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <event/thread.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <paho_mqtt.h>
#include <dht.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <MQTTClient.h>
#include <irq.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <thread.h>

atomic_int atomic_var = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&atomic_var, 1);
}

int main(void)
{
    unsigned state = irq_disable();

    // Initialize event timer for message events
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Add an event to the event timer
    evtimer_event_t event;
    evtimer_event_init_msg(&event, interrupt_handler, NULL);
    evtimer_add_msg(&evtimer, &event, 1000);

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    xtimer_sleep(2);

    // Check the atomic variable
    if (atomic_load(&atomic_var) > 0) {
        printf("Interrupt handling and atomic variable verification successful!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
