#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 5)  // Define the GPIO pin where DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_A, 10) // Define the GPIO pin where LED is connected
#define TEMP_THRESHOLD 30            // Temperature threshold in Celsius

void read_dht_and_control_led(void) {
    int16_t temperature = 0;
    int16_t humidity = 0;
    int16_t result = 0;

    // Assume dht_read function reads the values and assigns them to temperature
    result = dht_read(DHT_PIN, &humidity, &temperature);
    if (result == 0) { // Check if reading successful
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN); // Turn on the LED if temperature exceeds the threshold
        } else {
            gpio_clear(LED_PIN); // Turn off the LED if temperature is below the threshold
        }
    } else {
        printf("DHT reading failed\n");
    }
}

int main(void) {
    // Initialize DHT and GPIO pins
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht_and_control_led();
        xtimer_usleep(5000000); // Wait for 5 seconds before the next read
    }
    return 0;
}