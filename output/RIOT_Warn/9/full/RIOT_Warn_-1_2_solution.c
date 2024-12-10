#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN 4
#define LED_PIN 2
#define TEMP_THRESHOLD 30

void initialize_gpio(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

void check_temperature_and_activate_led(void) {
    int16_t temp, hum;
    while (dht_init(&dht, DHT11_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        xtimer_usleep(1000000); // Retry every 1 second
    }
    if (dht_read(&dht, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read data from DHT sensor\n");
    }
}

int main(void) {
    puts("Starting the DHT11 temperature monitor");
    initialize_gpio();

    while (1) {
        check_temperature_and_activate_led();
        xtimer_usleep(5000000); // Wait for 5 seconds before next read
    }
    return 0;
}