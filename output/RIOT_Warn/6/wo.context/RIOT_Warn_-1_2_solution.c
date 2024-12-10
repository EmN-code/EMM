#include <stdio.h>
#include <stdlib.h>
#include <led.h> // To control the on-board LED
#include <dht.h> // The DHT library for reading the sensor
#include <xtimer.h> // Timer library for scheduling

#define DHT_PIN GPIO_PIN(x, y) // Assume x and y are the specific identifiers for the ESP32 board pin connected to the DHT11
#define LED_PIN GPIO_PIN(x, z) // Assume x and z are the specific identifiers for the ESP32 board pin connected to the LED
#define TEMP_THRESHOLD 30 // Temperature threshold for LED activation

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(DHT_PIN, &temp, &hum) != 0) {
        printf("Failed to read DHT11 sensor\n");
        return;
    }

    printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);

    if (temp / 10 >= TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    }
    else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    // Initialize the GPIO pins
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    // Read the DHT11 every 5 seconds
    while (1) {
        read_dht11();
        xtimer_sleep(5);
    }

    return 0;
}