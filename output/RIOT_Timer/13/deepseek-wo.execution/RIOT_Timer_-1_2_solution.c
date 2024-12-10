
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <timex.h>
#include <evtimer_msg.h>

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO0, pin 13 for the LED
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t timer;
static mbox_t mbox;
static msg_t msg_array[8];

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the message box
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the event timer
    evtimer_init_msg(&timer, &mbox);

    // Set the timer to trigger every second
    evtimer_add_msg(&timer, timex_set(TIMER_INTERVAL, 0), timer_callback, NULL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        ((msg_t_handler)msg.content.ptr)(msg.content.ptr);
    }

    return 0;
}
