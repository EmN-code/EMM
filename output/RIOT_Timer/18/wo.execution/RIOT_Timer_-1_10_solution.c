#include <ztimer.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>
#include <unistd.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <periph/rtc.h>
#include <dht_params.h>
#include <dht.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <evtimer_mbox.h>

#define LED_PIN GPIO_PIN(0, 2) // Adjust according to your board's LED pin

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void)
{
    // Initialize the GPIO
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Set up a periodic timer with a 1-second interval
    static ztimer_t timer;
    ztimer_periodic_init(&timer, ZTIMER_MSEC, timer_callback, NULL, 1000);
    ztimer_periodic_start(&timer);

    // Start the event loop
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (true) {
        msg_receive(NULL);
    }

    return 0;
}