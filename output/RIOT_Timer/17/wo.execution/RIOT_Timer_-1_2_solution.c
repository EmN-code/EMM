#include "xtimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "thread.h"

#define LED_PIN GPIO_PIN(PORT_A, 10)

static evtimer_t evtimer;
static evtimer_event_t event;

void led_toggle_cb(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize LED_PIN as an output pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Setup event
    evtimer.event.base = xtimer_now();
    evtimer.event.callback = &led_toggle_cb;
    evtimer.event.period = 1000000U;  // 1 second
    
    // Initialize event timer
    evtimer_init_msg(&evtimer);
    
    // Make the evtimer event point to our event struct
    evtimer_add(&evtimer, &event, 0);

    // Main loop
    while (1) {
        evtimer_run(&evtimer);
        // Sleep to reduce energy consumption
        thread_yield();
    }
    return 0;
}