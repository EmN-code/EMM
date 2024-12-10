#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "mqttclient.h"
#include "paho_mqtt.h"
#include "ipv6/in.h"
#include "ipv6/addr.h"
#include "ipv6/esp.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define DHT_PIN         GPIO_PIN(0, 0)  // Adjust to your board's setup
#define LED_PIN         GPIO_PIN(0, 1)  // Adjust to your board's setup
#define TEMP_THRESHOLD  30  // Temperature threshold in Celsius

void *dht_sensor_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    dht_t dev;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("DHT reading failed\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("LED ON: Temperature exceeds threshold!\n");
            } else {
                gpio_clear(LED_PIN);
                printf("LED OFF: Temperature below threshold.\n");
            }
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);  // Wait for 5 seconds
    }

    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    char stack[THREAD_STACKSIZE_MAIN];

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_sensor_thread,
                  NULL, "dht_sensor_thread");

    return 0;
}