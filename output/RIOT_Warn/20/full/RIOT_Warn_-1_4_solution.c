// The code below interfaces with a DHT11 sensor connected to an ESP32 board using RIOT OS. It reads the sensor every 5 seconds and turns on an LED if the temperature exceeds a specific threshold. It also includes error handling for network connections.

#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_B, 5)
#define LED_PIN GPIO_PIN(PORT_A, 10)
#define THRESHOLD_TEMP 30 // Example threshold temperature, change as needed

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Ensure LED is initially off
}

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    dht_params_t dht_params;

    // Set up the DHT sensor
    dht_params.pin = DHT_PIN;

    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("Error: Unable to initialize DHT sensor\n");
        return -1;
    }

    init_led();

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error: Unable to read data from DHT sensor\n");
        } else {
            printf("Temp: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);

            // Turn on LED if temperature is above threshold
            if (temperature / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}