
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <time_units.h>
#include <stddef.h>
#include <MQTTClient.h>
#include <irq.h>
#include <thread.h>

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static atomic_int atomic_var = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&atomic_var, 1);
    printf("Interrupt handled, atomic_var incremented to %d\n", atomic_var);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize event timer for mbox events
    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    // Set up an event to trigger the interrupt handler
    evtimer_add_msg(&evtimer, xtimer_now_usec64() + US_PER_SEC, &mbox, NULL);

    // Enable interrupts
    unsigned state = irq_enable();

    // Main loop to handle events
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        interrupt_handler(NULL);
    }

    // Restore interrupt state
    irq_restore(state);

    return 0;
}
