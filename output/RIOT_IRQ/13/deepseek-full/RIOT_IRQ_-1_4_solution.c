
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <stddef.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

// Define a structure to hold atomic variables
typedef struct {
    atomic_int interrupt_handled;
    atomic_int interrupt_enabled;
} atomic_state_t;

// Function to simulate interrupt handling
void handle_interrupt(void)
{
    atomic_state_t *state = (atomic_state_t *)msg_reply_payload();
    atomic_store(&state->interrupt_handled, 1);
    printf("Interrupt handled\n");
}

int main(void)
{
    atomic_state_t state = {ATOMIC_VAR_INIT(0), ATOMIC_VAR_INIT(0)};

    // Initialize the event timer
    evtimer_msg_t et;
    msg_t msg;
    msg_t msg_array[8];
    evtimer_msg_init(&et, msg_array, 8);

    // Set up the event timer to trigger an interrupt
    xtimer_t timer;
    timer.target = timer.long_target = xtimer_now_timex() + timex_set(1, 0);
    timer.callback = (xtimer_callback_t)handle_interrupt;
    xtimer_set(&timer, 1 * US_PER_SEC);

    // Disable interrupts
    atomic_store(&state.interrupt_enabled, 0);

    // Wait for the interrupt to be handled
    while (atomic_load(&state.interrupt_handled) == 0) {
        xtimer_usleep(100);
    }

    // Restore interrupts
    atomic_store(&state.interrupt_enabled, 1);

    // Verify if the interrupt was handled correctly
    if (atomic_load(&state.interrupt_handled) == 1) {
        printf("Interrupt handling verified successfully\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
