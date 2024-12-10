
#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "unistd.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdlib.h"
#include "kernel_defines.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "string.h"
#include "periph/gpio.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "dht.h"
#include "dht_params.h"
#include "periph/i2c.h"
#include "periph/rtc.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the periodic callback to trigger every INTERVAL_MS milliseconds
    xtimer_set_periodic(&timer, INTERVAL_MS, (xtimer_callback_t)periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Sleep to prevent busy-waiting
        xtimer_sleep(1);
    }

    return 0;
}
