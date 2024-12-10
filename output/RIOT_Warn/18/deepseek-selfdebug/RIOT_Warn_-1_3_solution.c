
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 4)  // GPIO4 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)  // GPIO2 on ESP32
#define TEMP_THRESHOLD      30              // Temperature threshold in Celsius
#define INTERVAL            5               // Interval in seconds

static dht_t dht_dev;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN);

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_init();

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
