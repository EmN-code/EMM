#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "stddef.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "ismport/goto.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "timex.h"
#include "time_units.h"
#include "ztimer.h"
#include "ztimer.h"
#include "esp_system.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "periph/rtc.h"
#include "mutex.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "unistd.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include <string.h>

#define TIMER_INTERVAL (1U * 1000000U) // 1 second in microseconds
#define LED_PIN GPIO_PIN(0, 2) // Example GPIO pin where the LED is connected

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

void *led_blink_thread(void *arg) {
    xtimer_set(&timer, TIMER_INTERVAL);
    while (1) {
        timer_callback(NULL);
        xtimer_usleep(TIMER_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("Starting LED blink example\n");
    int res = gpio_init(LED_PIN, GPIO_OUT);
    if (res < 0) {
        printf("Error initializing GPIO\n");
        return 1;
    }
    gpio_clear(LED_PIN);

    pthread_t thread;
    pthread_create(&thread, NULL, led_blink_thread, NULL);
    pthread_join(thread, NULL);

    return 0;
}