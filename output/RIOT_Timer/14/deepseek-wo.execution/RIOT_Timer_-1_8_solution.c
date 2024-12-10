
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <esp_system.h>
#include <mutex.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static xtimer_t timer;
static volatile bool led_state = false;

void timer_callback(void) {
    led_state = !led_state;  // Toggle LED state
    printf("LED state: %s\n", led_state ? "ON" : "OFF");
    xtimer_set(&timer, TIMER_INTERVAL);  // Reset the timer
}

int main(void) {
    // Initialize the timer
    timer.callback = timer_callback;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop to keep the program running
    while (1) {
        xtimer_usleep(100000);  // Sleep for 100ms to avoid busy-waiting
    }

    return 0;
}
