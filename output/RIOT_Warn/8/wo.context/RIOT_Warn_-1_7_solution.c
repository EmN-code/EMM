#include <dht.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define DHT_PIN GPIO_PIN(PORT_A, 2) // Replace PORT_A with the actual port number
#define LED_PIN GPIO_PIN(PORT_B, 3) // Replace PORT_B with the actual port number
#define TEMP_THRESHOLD 30 // Temperature threshold in degrees Celsius
#define INTERVAL 5 * 1000000 // 5 seconds interval in microseconds

void init_gpio(void);
void read_temperature(void);

int main(void) {
    // Initialize GPIO Pins
    init_gpio();

    // Read temperature at regular intervals
    while (1) {
        read_temperature();
        xtimer_sleep(INTERVAL / 1000000); // Convert microseconds to seconds for xtimer_sleep
    }
    return 0;
}

void init_gpio(void) {
    // Initialize LED Pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT11
    if (dht_init(&dht, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
    }
}

void read_temperature(void) {
    int16_t temperature = 0, humidity = 0;
    int res = dht_read(&dht, &temperature, &humidity);

    if (res == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);

        // Check if temperature exceeds threshold
        if (temperature / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }
}