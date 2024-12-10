#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <msg.h>
#include <mbox.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

// Define GPIO pin for LED
#define LED_PIN GPIO_PIN(PORT_A, 0)

void toggle_led(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void)
{
    // Initialize GPIO Pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Define and initialize timer
    ztimer_t timer;
    timer.callback = toggle_led;
    timer.arg = NULL;

    // Configure timer to trigger every 1 second
    const uint32_t interval = US_PER_SEC;

    while (1) {
        ztimer_set(ZTIMER_USEC, &timer, interval);
        ztimer_sleep(ZTIMER_USEC, interval);
    }

    return 0;
}