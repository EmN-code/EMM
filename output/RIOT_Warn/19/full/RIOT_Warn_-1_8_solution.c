#include <stdio.h>
#include <xtimer.h>
#include <string.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN         GPIO_PIN(PORT_X, 0)   // Correct GPIO for DHT11 data
#define LED_PIN         GPIO_PIN(PORT_X, 1)   // Correct GPIO for LED control
#define TEMP_THRESHOLD  30       // Temperature threshold in Celsius
#define DELAY           (5U * US_PER_SEC) // 5 seconds delay

static dht_t dev;

void initialize_dht(void) {
    int res;
    while (1) {
        res = dht_init(&dev, DHT_MODEL_DHT11, DHT_PIN);
        if (res != DHT_OK) {
            printf("Failed to initialize DHT sensor. Error: %i\n", res);
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("DHT sensor initialized successfully.\n");
            break;
        }
    }
}

void read_and_check_temperature(void) {
    int16_t temperature, humidity;
    int res;

    while (1) {
        res = dht_read(&dev, &temperature, &humidity);
        if (res != DHT_OK) {
            printf("Failed to read from DHT sensor. Error: %i\n", res);
            xtimer_sleep(1);
            continue;
        }

        printf("Temperature: %2.1f°C, Humidity: %2.1f%%\n", temperature / 10.0, humidity / 10.0);
        if (temperature / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Temperature threshold exceeded. LED turned ON.\n");
        } else {
            gpio_clear(LED_PIN);
            printf("Temperature within safe limits.\n");
        }

        xtimer_usleep(DELAY);
    }
}

int main(void) {
    printf("Starting DHT11 temperature monitoring\n");
    gpio_init(LED_PIN, GPIO_OUT);
    initialize_dht();
    read_and_check_temperature();
    return 0;
}