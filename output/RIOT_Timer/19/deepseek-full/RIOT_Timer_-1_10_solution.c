
#include "net/gcoap.h"
#include "esp_system.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "dht_params.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "esp_wifi.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "net/sock/udp.h"
#include "mma8x5x_params.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    // Simulate toggling the LED
    printf("LED toggled\n");
    ztimer_set(ZTIMER_USEC, (ztimer_t *)arg, TIMER_INTERVAL);
}

int main(void) {
    ztimer_t timer;

    // Initialize the timer
    ztimer_init(&timer);

    // Set the timer to trigger every second
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Sleep for a while to avoid busy-waiting
        xtimer_usleep(100 * US_PER_MS);
    }

    return 0;
}
