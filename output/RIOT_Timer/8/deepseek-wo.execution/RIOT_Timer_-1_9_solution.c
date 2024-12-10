
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
#include <esp_wifi.h>
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

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED logic here
    printf("LED toggled\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Sleep to prevent busy-waiting
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}
