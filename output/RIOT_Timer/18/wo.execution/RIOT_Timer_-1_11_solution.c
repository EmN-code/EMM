#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <dht_params.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <esp_system.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define LED_PIN  GPIO_PIN(PORT_B, 0)  // Example LED pin

void *led_thread(void *arg) {
    (void)arg;
    
    gpio_init(LED_PIN, GPIO_OUT);
    
    ztimer_t timer = ZTIMER_INITIALIZER;

    while (1) {
        gpio_toggle(LED_PIN);
        ztimer_periodic_wakeup(ZTIMER_USEC, &timer, INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("Starting LED blink example with periodic timer\n");

    // Start the LED blinking thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, led_thread, NULL, "led_thread");

    return 0;
}