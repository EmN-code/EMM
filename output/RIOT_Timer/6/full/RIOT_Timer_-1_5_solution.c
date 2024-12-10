#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <msg.h>
#include <mbox.h>
#include <timex.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>

#define LED_PIN 2 // Assume GPIO2 is connected to the LED
#define TIMER_INTERVAL (1U * US_PER_SEC)

static evtimer_t evt;

void timer_callback(void *arg)
{
    (void)arg;
    // Imagine this toggles the LED state:
    // e.g., gpio_toggle(LED_PIN);
    printf("Timer triggered\n");
}

int main(void)
{
    msg_init_queue((msg_t *)malloc(sizeof(msg_t) * 8), 8);

    // Initial setup for the evtimer
    evtimer_init_msg(&evt);
    evtimer_set(&evt, TIMER_INTERVAL, timer_callback, NULL);
    evtimer_start(&evt);

    while (1) {
        pause(); // await any signals
    }
    return 0;
}