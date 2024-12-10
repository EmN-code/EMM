#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_B, 5)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30   //temperature threshold in degree Celsius
#define INTERVAL (5U * US_PER_SEC)

static dht_t dht_dev;

int main(void) {
    // Initialize DHT11 sensor
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    if (dht_init(&dht_dev, &dht_params) != 0) {
        printf("DHT sensor initialization failed\n");
        return -1;
    }

    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("LED initialization failed\n");
        return -1;
    }

    int16_t temp, hum;

    while (1) {
        // Read data from DHT sensor
        if (dht_read(&dht_dev, &temp, &hum) != 0) {
            printf("Failed to read from DHT sensor\n");
        }
        else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            if (temp > TEMP_THRESHOLD * 10) { // since DHT11 returns temp in 10ths
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Wait for the next interval
        xtimer_sleep(INTERVAL);
    }
    return 0;
}
