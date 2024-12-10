
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "msg.h"
#include "ztimer.h"
#include "mbox.h"
#include "time_units.h"
#include "evtimer_msg.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN GPIO_PIN(0, 16)          // Example LED pin for ESP32

static msg_t msg_queue[8];
static mbox_t mbox;

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void)
{
    // Initialize the message queue and mailbox
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer
    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
