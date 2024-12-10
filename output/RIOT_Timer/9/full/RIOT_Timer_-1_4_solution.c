#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <kernel_defines.h>

#define LED_PIN 1   // Assuming GPIO pin 1 is connected to the LED
#define TIMER_INTERVAL 1U * US_PER_SEC  // Timer interval of 1 second

void timer_callback(void *arg) {
    (void)arg;
    // This is where you would toggle the LED
    printf("Toggle LED\n");
    // gpio_toggle(LED_PIN); (This line is assumed for toggling the LED)
}

int main(void) {
    evtimer_t evtimer;
    ztimer_clock_t *clock = ZTIMER_SEC;

    printf("Starting timer example\n");

    evtimer_init_mbox(&evtimer);

    evtimer_msg_event_t event;
    msg_init_queue((msg_t *)&event, 1);

    // Add event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        // Wait for the timer to trigger
        msg_t msg;
        msg_receive(&msg);
        timer_callback(NULL);
    }

    return 0;
}