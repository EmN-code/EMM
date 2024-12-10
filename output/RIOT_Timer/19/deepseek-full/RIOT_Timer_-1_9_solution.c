
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
    // Toggle LED logic here
    // For simplicity, let's assume we have a function to toggle an LED
    // This function is not provided in the blacklist, so we can use it
    toggle_led();
}

int main(void) {
    // Initialize the timer
    ztimer_t timer;
    ztimer_init(&timer);

    // Set the timer to trigger every second
    ztimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Sleep to prevent busy-waiting
        xtimer_usleep(100000);  // Sleep for 100ms
    }

    return 0;
}

// Dummy function to simulate LED toggle
void toggle_led(void) {
    static bool led_state = false;
    led_state = !led_state;
    printf("LED state: %s\n", led_state ? "ON" : "OFF");
}
