#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <periph/dht.h>

#define DHT_PIN GPIO_PIN(0, 4) // Assuming DHT11 is connected to GPIO pin 4
#define LED_PIN GPIO_PIN(0, 15) // Assuming LED is connected to GPIO pin 15
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius
#define READ_INTERVAL 5 // DHT11 read interval in seconds

static dht_t dev;

void read_sensor(void) {
    int16_t temp, humidity;
    int ret = dht_read(&dev, &temp, &humidity);
    if (ret != DHT_OK) {
        printf("Error reading from DHT sensor: %d\n", ret);
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, humidity / 10, humidity % 10);

    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
        printf("Warning: Temperature exceeds threshold!\n");
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_sensor();
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}