#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 5)
#define LED_PIN GPIO_PIN(PORT_B, 7)
#define TEMP_THRESHOLD 30
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

dht_t dev;

void init_dht(void) {
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("DHT initialization failed\n");
        while (1);
    }
}

void check_temperature(void) {
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read data from sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    // Initialize GPIO pin for LED
    gpio_init(LED_PIN, GPIO_OUT);
    // Initialize DHT sensor
    init_dht();
    // Check temperature periodically
    check_temperature();
    return 0;
}