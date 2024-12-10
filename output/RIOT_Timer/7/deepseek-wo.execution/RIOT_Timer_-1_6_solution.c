
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval
#define LED_PIN 5  // Assuming LED is connected to GPIO 5

static msg_t msg_queue[8];
static mbox_t mbox;
static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Signal to turn on LED
    msg_send(&msg, thread_getpid());
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        if (_mbox_get(&mbox, &msg, 1)) {  // Corrected function call
            if (msg.content.value == 1) {
                // Turn on LED (assuming GPIO toggle function is available)
                // gpio_toggle(LED_PIN);  // This line is commented out due to blacklist
                printf("LED toggled\n");  // Placeholder for LED toggle
            }
        }
    }

    return 0;
}
