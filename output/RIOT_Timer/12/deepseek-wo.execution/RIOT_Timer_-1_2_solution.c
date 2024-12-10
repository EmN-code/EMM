
#include <stdio.h>
#include <xtimer.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <ztimer.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;
static mbox_t mbox;
static msg_t msg_array[8];

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered, turning LED on\n");
    // Assuming LED is connected to GPIO pin 5
    gpio_set(GPIO_PIN(0, 5));
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer, &mbox);

    // Set up the timer to trigger every second
    evtimer_add_msg(&evtimer, timex_set(0, TIMER_INTERVAL), timer_callback, NULL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        ((msg_t_handler)msg.content.ptr)(msg.content.ptr);
    }

    return 0;
}
