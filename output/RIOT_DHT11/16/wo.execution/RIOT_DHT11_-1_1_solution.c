#include "thread_arch.h"
#include "dht.h"
#include "esp_idf_gpio_config.h"

#define DHT_TYPE DHT11
#define GPIO_PIN 2

int main(void) {
    esp_err_t res;
    dht_t dev;
    dht_params_t params = {
        .pin = GPIO_PIN,
        .type = DHT_TYPE
    };

    // Initialize the DHT sensor
    if (dht_init(&dev, &params) != 0) {
        puts("DHT initialization failed");
        return 1;
    }

    printf("DHT initialized, reading every 5 seconds\n");

    while (1) {
        int16_t temperature, humidity;

        // Read the sensor
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            puts("DHT reading failed");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, abs(temperature % 10),
                   humidity / 10, abs(humidity % 10));
        }

        // Sleep for 5 seconds
        thread_sleep(5);
    }

    return 0;
}