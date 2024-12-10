#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT11_PIN GPIO_PIN(0, 1) // Specify the correct GPIO pin
#define LED_PIN GPIO_PIN(0, 2)   // Specify the correct GPIO pin for the LED
#define TEMP_THRESHOLD 30        // Temperature threshold for warning

static dht_t dev;

void init_dht(void) {
    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("DHT11 initialization failed\n");
    }
    else {
        printf("DHT11 initialized successfully\n");
    }
}

void read_dht_and_control_led(void) {
    int16_t temperature;
    int16_t humidity;
    if (dht_read(&dev, &humidity, &temperature) != DHT_OK) {
        printf("Failed to read from DHT11\n");
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temperature / 10, temperature % 10,
           humidity / 10, humidity % 10);

    if (temperature / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
        printf("Warning: High temperature detected!\n");
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    init_dht();

    while (1) {
        read_dht_and_control_led();
        xtimer_sleep(5);
    }

    return 0;
}