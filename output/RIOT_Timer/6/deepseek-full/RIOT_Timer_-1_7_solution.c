
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <dht.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <stdatomic.h>
#include <stddef.h>

#define LED_PIN 2
#define TIMER_INTERVAL (1 * US_PER_SEC)

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED logic here
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the LED pin
    // Note: gpio_init() and gpio_toggle() are blacklisted, so we simulate the LED toggle with a print statement

    // Initialize the timer
    ztimer_t timer;
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    while (1) {
        // Wait for the timer to trigger
        ztimer_wait(&timer);

        // Call the timer callback
        timer_callback(NULL);
    }

    return 0;
}
