#include <thread_arch.h>
#include <dht.h>
#include <esp_idf_gpio_reset_pin.h>
#include <tim.h>
#include <stdio.h>

#define DHT_PIN GPIO_NUM_2

void dht_read_and_print(void) {
    int16_t temperature = 0;
    int16_t humidity = 0;
    dht_t dev;
    
    // Initialize the DHT11 sensor.
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read DHT11\n");
        } else {
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temperature / 10.0, humidity / 10.0);
        }
        
        // Wait for 5 seconds
        thread_sleep(5000);
    }
}

int main(void) {
    printf("Starting DHT11 reading on GPIO %d every 5 seconds...\n", DHT_PIN);

    dht_read_and_print();

    return 0;
}
