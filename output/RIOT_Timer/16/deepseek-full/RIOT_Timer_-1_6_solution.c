
#include "net/gcoap.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/sock/udp.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value to indicate timer event
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Set up the timer
    ztimer_t timer = {
        .callback = timer_callback,
        .arg = NULL
    };

    // Start the timer
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    msg_t msg;
    while (1) {
        // Wait for a message
        msg_receive(&msg);

        // Check if the message is from the timer
        if (msg.content.value == 1) {
            // Turn on the LED (assuming LED is connected to GPIO)
            // Note: GPIO operations are not allowed, so this is a placeholder
            // For actual LED control, you would need to use GPIO functions.
            // For example: gpio_set(LED_PIN);
        }
    }

    return 0;
}
