#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <periph/dht.h>

#define GPIO_DHT_PIN GPIO_PIN(0, 4)  // Assume GPIO0_4 for DHT11
#define GPIO_LED_PIN GPIO_PIN(0, 5)  // Assume GPIO0_5 for LED
#define TEMP_THRESHOLD 30  // Temperature threshold in degree Celsius
#define SLEEP_INTERVAL 5   // Seconds

dht_t dht_dev;

int read_temp_and_control_led(void) {
    int16_t temp, hum;
    int result = dht_read(&dht_dev, &temp, &hum);

    if (result != DHT_OK) {
        printf("Failed to read from DHT11 sensor\n");
        return -1;
    }
    printf("Temp: %dC, Hum: %d%%\n", temp / 10, hum / 10);

    if ((temp / 10) > TEMP_THRESHOLD) {
        gpio_set(GPIO_LED_PIN);
        printf("Temperature exceeds threshold. LED turned ON\n");
    } else {
        gpio_clear(GPIO_LED_PIN);
        printf("Temperature is within safe range. LED turned OFF\n");
    }
    return 0;
}

int main(void) {
    // Initialize DHT11 and LED pin
    if (dht_init(&dht_dev, GPIO_DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return -1;
    }
    if (gpio_init(GPIO_LED_PIN, GPIO_OUT)) {
        printf("Failed to initialize GPIO as output\n");
        return -1;
    }

    while (1) {
        read_temp_and_control_led();
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}