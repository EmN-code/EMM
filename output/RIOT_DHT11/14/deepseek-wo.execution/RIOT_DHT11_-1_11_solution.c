
#include "periph/gpio.h"
#include "xtimer.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

typedef struct {
    gpio_t pin;
    uint8_t type;
} dht_t;

int dht_read(dht_t *dev, int16_t *temp, int16_t *hum) {
    // Simulated DHT11 read function
    *temp = 25; // Simulated temperature value
    *hum = 60;  // Simulated humidity value
    return 0;   // Success
}

int main(void) {
    dht_t dht11;
    int16_t temperature, humidity;

    dht11.pin = DHT_PIN;
    dht11.type = 0; // Assuming 0 for DHT11 type

    while (1) {
        if (dht_read(&dht11, &temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
