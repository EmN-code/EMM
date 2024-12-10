#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>

#define LED_PIN GPIO_PIN(PORT_A, 2)

static void timer_event_handler(etimer_t *timer, void *arg) {
    (void)timer;
    (void)arg;
    // Toggle LED
    gpio_toggle(LED_PIN);
    printf("LED toggled!\n");
}

int main(void) {
    // Initialize GPIO
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Initialize evtimer
    evtimer_t evtimer;
    evtimer_init_mbox(&evtimer);

    // Configure periodic timer to trigger every second
    evtimer_event_t event;
    ztimer_periodic_wakeup(ZTIMER_MSEC, &event.event, 1000);
    evtimer_add(&evtimer, &event);

    printf("Starting event loop\n");

    while (1) {
        evtimer_run(&evtimer);
        xtimer_sleep(1);
    }

    return 0;
}
