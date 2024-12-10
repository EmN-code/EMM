#include "xtimer.h"
#include "stdio.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2) // DHT11 connected to GPIO2
#define LED_PIN GPIO_PIN(0, 3) // LED connected to GPIO3
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius

static dht_t dht_dev;

void read_dht_and_control_led(void) {
    int16_t temp, hum;

    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

        if (temp / 10 > TEMP_THRESHOLD) {
            // Turn on the LED
            gpio_set(LED_PIN);
        } else {
            // Turn off the LED
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    // Initialize DHT
    dht_init(&dht_dev, DHT_PIN);

    while (1) {
        read_dht_and_control_led();
        xtimer_sleep(5);
    }
    return 0;
}