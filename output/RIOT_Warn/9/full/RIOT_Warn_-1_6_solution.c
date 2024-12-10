#include <ztimer.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <netinet/in.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <dht.h>
#include <msg.h>
#include <time_units.h>
#include <timex.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <unistd.h>
#include <thread.h>

define LED_PIN GPIO_PIN(PORT_A, 2)
#define DHT_PIN GPIO_PIN(PORT_A, 1)
#define TEMP_THRESHOLD 30

void *temp_check(void *arg) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("DHT initialization failed\n");
        return NULL;
    }

    // Initialize LED GPIO
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Failed to initialize LED GPIO\n");
        return NULL;
    }

    while (1) {
        // Attempt to read from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            // Check if the temperature exceeds the threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    thread_t temp_thread;

    thread_create((char *)temp_thread, sizeof(temp_thread), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, temp_check, NULL, "temp_check");

    return 0;
}