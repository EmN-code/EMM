#include <stdio.h>
#include <dht_params.h>
#include <dht.h>
#include <xtimer.h>
#include <esp_idf_gpio_config.h>

#define DHT_PIN GPIO_NUM_2  // GPIO2 where the DHT11 is connected
#define DHT_TYPE DHT11      // Define the sensor type

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    dht_params_t params;

    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    // Configuring the GPIO
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << DHT_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    esp_idf_gpio_config(&io_conf);

    // Initialize the sensor with the given parameters
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    int16_t temperature, humidity;

    while(1) {
        // Read the sensor
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d °C, Humidity: %d.%d %%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}