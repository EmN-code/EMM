
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 is connected to an LED on ESP32
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void _led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer message event
    evtimer_msg_t timer_event;
    msg_t msg;
    msg_init_event(&msg, (void *)&timer_event, 0);

    // Set up the timer to trigger every second
    evtimer_add_msg(&timer_event, TIMER_INTERVAL, &msg, (void *)_led_toggle);

    while (1) {
        // Wait for the timer event
        msg_receive(&msg);
        // Execute the callback function (toggle LED)
        ((void (*)(void))msg.content.ptr)();
    }

    return 0;
}
