#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 4)  // Replace with the actual pin
#define LED_PIN GPIO_PIN(0, 2)  // Replace with the actual pin
#define TEMP_THRESHOLD 30       // Temperature threshold in °C

// Function to initialize the DHT sensor
int dht_initialize(gpio_t pin) {
    int res = dht_init(DHT_TYPE_DHT11);
    if (res != 0) {
        printf("Failed to initialize DHT sensor\n");
    }
    return res;
}

// Function to read temperature from DHT sensor
int read_temperature(int *temperature) {
    int humidity = 0;  // Placeholder for humidity value as dht_read() requires it
    int res = dht_read(DHT_PIN, temperature, &humidity);
    if (res != 0) {
        printf("Failed to read from DHT sensor, error: %d\n", res);
    }
    return res;
}

// Function to control the LED based on temperature
void control_led(int temperature) {
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    if (dht_initialize(DHT_PIN) != 0) {
        return 1;  // Exit if DHT initialization fails
    }

    while (1) {
        int temperature = 0;

        // Try reading temperature
        if (read_temperature(&temperature) == 0) {
            printf("Current temperature: %d°C\n", temperature);
            control_led(temperature);
        }

        // Sleep for 5 seconds before next read
        xtimer_sleep(5);
    }
    return 0;
}