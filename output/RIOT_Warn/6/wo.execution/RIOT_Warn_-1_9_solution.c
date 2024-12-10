#include <ztimer.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define DHT_PIN 4  // Example GPIO pin for DHT11
#define LED_PIN 2  // Example GPIO pin for LED
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

void read_dht_and_control_led(void) {
    int16_t temperature = 0, humidity = 0;
    dht_t dev;
    dev.pin = DHT_PIN;

    if (dht_read(&dev, &temperature, &humidity) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, abs(temperature % 10), humidity / 10, abs(humidity % 10));
        if (temperature / 10 > TEMP_THRESHOLD) {
            // Turn on LED because temperature threshold is exceeded
            gpio_set(LED_PIN);
        } else {
            // Turn off LED
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}

int main(void) {
    // Initialize GPIO
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    while (1) {
        read_dht_and_control_led();
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5000 milliseconds
    }

    return 0;
}
