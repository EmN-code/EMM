#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "timex.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(PORT_B, 5) // DHT11 data pin
#define LED_PIN GPIO_PIN(PORT_A, 7) // LED pin
#define TEMP_THRESHOLD 30 // temperature threshold in Celsius
#define DHT_TYPE DHT11
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static dht_t dev;

void read_and_act_on_sensor(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("Error reading DHT11\n");
        return;
    }
    printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    // Initialize DHT11 sensor
    if (dht_init(&dev, &(dht_params_t){.pin = DHT_PIN, .type = DHT_TYPE}) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return -1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a periodic timer
    while (1) {
        read_and_act_on_sensor();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}