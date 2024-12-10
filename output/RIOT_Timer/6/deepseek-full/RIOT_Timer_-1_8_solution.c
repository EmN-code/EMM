
#include "esp_system.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/emcute.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    // Toggle LED logic here
    // For example, if you have an LED connected to a GPIO pin, you would toggle it here
    // Since gpio_toggle() is blacklisted, you need to implement your own LED toggle logic
    // This is just a placeholder for the LED toggle logic
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the timer
    ztimer_t timer;
    ztimer_init(&timer, ZTIMER_USEC, timer_callback, NULL);

    // Set the timer to trigger every second
    ztimer_set(&timer, TIMER_INTERVAL);

    // Main loop to keep the program running
    while (1) {
        // Sleep to prevent busy-waiting
        xtimer_usleep(100000);  // Sleep for 100 ms
    }

    return 0;
}
