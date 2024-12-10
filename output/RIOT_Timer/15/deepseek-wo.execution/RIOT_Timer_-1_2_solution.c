
#include <stdio.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <timex.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define LED_PIN             GPIO_PIN(0, 13)  // Assuming GPIO0, pin 13 for LED
#define TIMER_INTERVAL      (1 * US_PER_SEC) // 1 second interval

static evtimer_t timer;
static evtimer_mbox_event_t timer_event;
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

    // Initialize the message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&timer);

    // Set up the timer event
    timer_event.event.offset = 0;
    timer_event.event.interval = TIMER_INTERVAL;
    timer_event.event.callback = timer_callback;
    timer_event.event.arg = NULL;

    // Add the timer event to the event timer
    evtimer_add_mbox(&timer, &timer_event, &mbox);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        if (msg.content.ptr == &timer_event) {
            timer_callback(NULL);
        }
    }

    return 0;
}
