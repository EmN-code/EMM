#include "kernel_defines.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ztimer.h>
#include <evtimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <time_units.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#define LED_PIN GPIO_PIN(PORT_A, 5) // Adjust the port and pin in accordance with your setup

void timer_callback(void *arg) {
    (void)arg; // Unused
    gpio_toggle(LED_PIN);
    printf("LED Toggled\n");
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize GPIO\n");
        return 1;
    }

    // Initialize a timer event
    evtimer_t evtimer;
    evtimer.callback = timer_callback;
    evtimer.arg = NULL;

    evtimer_event_t event;
    evtimer.events = &event;
    evtimer.base = ztimer_now(ZTIMER_MSEC);

    // Setup event message
    msg_t msg;
    evtimer_event_init(&event, &evtimer, 1000 * US_PER_MS, &msg);

    // Set up the timer to fire every second
    evtimer_set(&evtimer);

    // Main loop
    while (1) {
        // Wait for message
        msg_receive(&msg);
    }

    return 0;
}