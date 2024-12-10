#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <stdatomic.h>
#include <stddef.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <ztimer.h>
#include <dht_params.h>
#include <evtimer_mbox.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <msg.h>
#include <stdlib.h>
#include <timex.h>
#include <unistd.h>
#include <timex.h>

// GPIO pin for the LED
#define LED_PIN GPIO_PIN(PORT_A, 0)  // Adjust according to your ESP32 configuration

static void evtimer_cb(void *arg)
{
    (void)arg;
    // Toggle LED each second
    printf("Toggling LED\n");
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED_PIN as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize GPIO\n");
        return (1);
    }
    
    // Set up the event timer, e.g., period = 1 second
    evtimer_t timer;
    evtimer_event_t event;

    evtimer_event_init(&event, evtimer_cb, NULL);
    evtimer_add(&timer, &event, 1 * US_PER_SEC, PERIODIC);

    while (1) {
        // Event loop to keep the main function alive
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}