#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Example setup, adjust according to the board
#define THRESHOLD 30 // Temperature threshold to trigger LED
#define DHT_PIN GPIO_PIN(PORT_B, 4) // Example pin setup for DHT11, adjust as needed

dht_t dev;

void init(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    // Initialize the DHT device
    dht_init(&dev, DHT_PIN);
}

void read_sensor_and_check_temperature(void) {
    int16_t temperature, humidity;
    if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        if (temperature > THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}

int main(void) {
    init();
    while (1) {
        read_sensor_and_check_temperature();
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return 0;
}