#include "ztimer.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "sys/socket.h"
#include "socket.h"
#include "pthread.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "esp_system.h"
#include "esp_wifi.h"

#define INTERVAL 1000000 // Interval for 1 second in microseconds

static ztimer_t timer;
static gpio_t led_pin = GPIO_PIN(PORT_B, 0); // Use appropriate pin for your setup

static void timer_cb(void *arg) {
    (void) arg;
    gpio_toggle(led_pin);
}

int main(void) {
    if (gpio_init(led_pin, GPIO_OUT) < 0) {
        puts("Error to initialize GPIO\n");
        return 1;
    }

    ztimer_init(ZTIMER_MSEC);
    ztimer_init(&timer);

    ztimer_cb_set(&timer, timer_cb, NULL);
    ztimer_set_periodic(&timer, INTERVAL);

    // Infinite loop to keep main from terminating
    while (1) {
        // Sleeping allows other threads to run
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}