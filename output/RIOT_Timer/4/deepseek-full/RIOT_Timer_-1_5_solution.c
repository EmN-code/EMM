
#include "mma8x5x_regs.h"
#include "net/sock/udp.h"
#include "assert.h"
#include "dht_params.h"
#include "evtimer_mbox.h"
#include "event/periodic_callback.h"
#include "periph/gpio.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "netinet/in.h"
#include "msg.h"
#include "thread.h"
#include "mma8x5x_params.h"
#include "evtimer_msg.h"
#include "periph/i2c.h"
#include "net/sock/util.h"
#include "event/thread.h"
#include "net/gcoap.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "periph/rtc.h"
#include "ztimer.h"
#include "timex.h"

#define LED_PIN GPIO_PIN(0, 13)  // Example LED pin for ESP32

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

static void led_toggle_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize and start the periodic callback event
    event_periodic_callback_create(&periodic_event, ZTIMER_MSEC, 1000, &event_queue, led_toggle_callback, NULL);

    // Start the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
