#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <dht.h>
#include <dht_params.h>
#include <periph/gpio.h>
#include <thread.h>
#include <evtimer_msg.h>

#define DHT11_PARAM DHT_PARAM_1
#define TEMP_THRESHOLD 30
#define LED_PIN GPIO_PIN(0, 19)
#define STACK_SIZE (1 << 10)

char stack[STACK_SIZE];

void *dht_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    gpio_init(LED_PIN, GPIO_OUT);

    if (dht_init(&dev, &DHT11_PARAM) != DHT_OK) {
        printf("DHT initialization failed\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", (temp / 10), abs(temp % 10), (hum / 10), abs(hum % 10));
            if (temp > TEMP_THRESHOLD * 10) {  // DHT11 gives temperature in tenths of a degree
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_USEC, 5000000); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");
    return 0;
}
