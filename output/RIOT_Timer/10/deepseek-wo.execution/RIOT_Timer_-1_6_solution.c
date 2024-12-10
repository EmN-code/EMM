
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Timer triggered!\n");
    // Assuming LED is connected to GPIO 5 on ESP32
    gpio_toggle(GPIO_PIN(0, 5));
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(GPIO_PIN(0, 5), GPIO_OUT);

    // Initialize xtimer
    xtimer_init();

    // Set up the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
