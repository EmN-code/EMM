#include <ztimer.h>
#include <unistd.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <dht.h>
#include <dht_params.h>
#include <esp_system.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)

int main(void) {
    printf("Starting DHT11 sensor read on ESP32\n");

    // Initialize DHT parameters
    dht_params_t my_dht_params;
    my_dht_params.pin = DHT_PIN;
    my_dht_params.type = DHT11;

    dht_t my_dht;
    dht_init(&my_dht, &my_dht_params);

    int16_t temperature, humidity;
    
    while (1) {
        if (dht_read(&my_dht, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}