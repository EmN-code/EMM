
#include "periph/gpio.h"
#include "xtimer.h"
#include "dht.h"
#include "thread.h"
#include "dht_params.h"

#define LED_PIN            GPIO_PIN(0, 18)  // Example LED pin for ESP32
#define DHT_PIN            GPIO_PIN(0, 19)  // Example DHT11 pin for ESP32
#define TEMP_THRESHOLD     25               // Temperature threshold in Celsius
#define READ_INTERVAL      5                // Read interval in seconds

static dht_t dht_dev;

void init_dht(void) {
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
    };
    dht_init(&dht_dev, &dht_params);
}

void read_dht(int16_t *temp, int16_t *hum) {
    if (dht_read(&dht_dev, temp, hum) != DHT_OK) {
        *temp = -1;  // Error reading temperature
    }
}

void control_led(int16_t temp) {
    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    int16_t temp, hum;

    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT11 sensor
    init_dht();

    while (1) {
        // Read temperature and humidity
        read_dht(&temp, &hum);

        // Control LED based on temperature
        control_led(temp);

        // Sleep for 5 seconds
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}
