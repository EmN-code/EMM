#include <stdio.h>
#include "thread_arch.h"
#include "thread_flags.h"
#include "esp_idf_gpio_config.h"
#include "dht.h"

#define GPIO_PIN 2
#define DHT_TYPE DHT_TYPE_DHT11

void *read_dht11_thread(void *arg) {
    dht_t dht_dev;
    dht_params_t params;
    int16_t temperature = 0, humidity = 0;

    // Initialize the DHT sensor
    params.pin = GPIO_PIN;
    params.type = DHT_TYPE;
    dht_init(&dht_dev, &params);
    
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %0.1f°C, Humidity: %0.1f%%\n", temperature / 10.0, humidity / 10.0);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(read_dht11_thread_stack, sizeof(read_dht11_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht11_thread, NULL, "read_dht11");
    return 0;
}