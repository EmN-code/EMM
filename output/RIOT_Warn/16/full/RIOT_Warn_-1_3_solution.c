#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "saul.h"
#include "saul_reg.h"

#define LED_PIN GPIO_PIN(0, 22) // Define the GPIO pin for the LED
#define DHT11_SENSOR_TYPE SAUL_SENSE_TEMP // Assume DHT11 is accessible via SAUL as a temperature sensor
#define TEMPERATURE_THRESHOLD 30 // Define temperature threshold for LED warning
#define DHT11_READ_INTERVAL (5U * US_PER_SEC) // Define read interval in microseconds

static int read_dht11_temperature(void);

int main(void) {
    // Initialize GPIO for LED
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error to initialize GPIO
");
        return 1;
    }

    printf("Starting DHT11 sensor demo\n");

    while (1) {
        // Attempt to read temperature
        int temp = read_dht11_temperature();
        if (temp < 0) {
            printf("Error reading temperature\n");
        } else {
            printf("Temperature: %d °C\n", temp);
            if (temp > TEMPERATURE_THRESHOLD) {
                printf("Temperature threshold exceeded, turning on LED\n");
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for the next read interval
        xtimer_usleep(DHT11_READ_INTERVAL);
    }

    return 0;
}

// Function definition with prototype to handle reading the temperature
static int read_dht11_temperature(void) {
    saul_reg_t *dev = saul_reg_find_type(DHT11_SENSOR_TYPE);
    if (!dev) {
        printf("Temperature sensor not found\n");
        return -1;
    }

    phydat_t res;
    int dim = saul_reg_read(dev, &res);
    if (dim < 1) {
        printf("Failed to read temperature data\n");
        return -1;
    }

    return res.val[0]; // Assuming the first value is the temperature
}