#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include "dht.h"  // Include the DHT sensor library for RIOT
#include "periph/gpio.h"  // Peripheral library for GPIO

#define DHT_PIN  GPIO_PIN(0, 0)  // DHT sensor data pin
#define LED_PIN  GPIO_PIN(0, 2)  // LED GPIO pin
#define TEMP_THRESHOLD  30  // Temperature threshold in Celsius
#define SLEEP_INTERVAL  (5U * US_PER_SEC)  // 5 seconds interval

int main(void) {
    dht_t sensor;
    int16_t temp, hum;

    dht_setup(&sensor, DHT_TYPE_DHT11, DHT_PIN);
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        if (dht_read(&sensor, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("LED On: Temperature above threshold!\n");
            } else {
                gpio_clear(LED_PIN);
                printf("LED Off: Temperature below threshold.\n");
            }
        } else {
            printf("DHT read error\n");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}