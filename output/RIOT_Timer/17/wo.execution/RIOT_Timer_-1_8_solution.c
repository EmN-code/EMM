#include <xtimer.h>
#include <stdio.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <mibx.h>
#include <msg.h>
#include <timex.h>
#include <time_units.h>
#include <thread.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <unistd.h>
#include <zetimer.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <esp_system.h>
#include <paho_mqtt.h>

#ifndef LED0_PIN
#define LED0_PIN GPIO_PIN(PORT_A, 5)
#endif

static void _timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED0_PIN);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    if (gpio_init(LED0_PIN, GPIO_OUT)) {
        printf("Error to initialize LED GPIO\n");
        return 1;
    }

    // Initialize the timer
    ztimer_t timer;
    timer_init(&timer, 1000, _timer_callback, NULL);

    // Start the timer
    timer_start(&timer);

    // Keep the main thread alive
    while (1) {
        thread_sleep();
    }

    return 0;
}