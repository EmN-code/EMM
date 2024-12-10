
#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define LED_PIN             GPIO_PIN(0, 13)  // Example GPIO pin for LED
#define TIMER_INTERVAL      (1 * US_PER_SEC) // 1 second interval

static xtimer_t timer;
static evtimer_msg_t evtimer;
static mbox_t mbox;
static msg_t msg_queue[8];

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the event timer
    evtimer_init_mbox(&evtimer, &mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Set up the periodic timer
    xtimer_set_periodic(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to handle events
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        // Handle the message if needed
    }

    return 0;
}
