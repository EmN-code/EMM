
#include <stdlib.h>
#include <stdio.h>
#include <time_units.h>
#include <xtimer.h>
#include <mutex.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/emcute.h>
#include <msg.h>
#include <ztimer.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static xtimer_t timer;
static uint8_t led_state = 0;

void timer_callback(void)
{
    led_state = !led_state;  // Toggle LED state
    printf("LED state: %d\n", led_state);
}

int main(void)
{
    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, (xtimer_callback_t)timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
