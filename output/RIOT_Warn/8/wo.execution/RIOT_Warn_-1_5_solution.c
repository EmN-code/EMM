#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "periph/gpio.h"
#include <stdio.h>

#define DHT_GPIO_PIN 5  // Example GPIO pin for DHT
#define LED_GPIO_PIN 18 // Example GPIO pin for LED
#define TEMP_THRESHOLD 30 // Temperature threshold for LED

void delay(int seconds) {
    int milli_seconds = 1000 * seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds)
        ;
}

int main(void) {
    int16_t temperature, humidity;
    // Init GPIO for LED
    gpio_init(LED_GPIO_PIN, GPIO_OUT);

    while (1) {
        // Simulate reading from DHT sensor
        int result = dht_read(&temperature, &humidity);
        if (result == 0) {  // Check if reading was successful
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            if (temperature > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(LED_GPIO_PIN);
            } else {
                // Turn off LED
                gpio_clear(LED_GPIO_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for 5 seconds
        delay(5);
    }

    return 0;
}
