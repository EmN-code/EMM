
#include <stdlib.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <event/thread.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <MQTTClient.h>
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

static void timer_callback(void *arg) {
    (void)arg;
    // Turn on LED logic here
    printf("LED turned on\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger every second
    xtimer_set_periodic(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to keep the thread alive
    while (1) {
        // Sleep to prevent busy-waiting
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
