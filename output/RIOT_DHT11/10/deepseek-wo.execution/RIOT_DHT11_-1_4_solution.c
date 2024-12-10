
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

void print_dht_data(int16_t temp, int16_t hum) {
    char temp_str[10];
    char hum_str[10];

    // Convert temperature and humidity to string
    snprintf(temp_str, sizeof(temp_str), "%d", temp);
    snprintf(hum_str, sizeof(hum_str), "%d", hum);

    // Print the data
    printf("Temperature: %s C, Humidity: %s %%\n", temp_str, hum_str);
}

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        puts("DHT11 sensor initialized successfully");
    } else {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            print_dht_data(temp, hum);
        } else {
            puts("Failed to read data from DHT11 sensor");
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
