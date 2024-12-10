#include "xtimer.h"
#include <stdio.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 10)  // Ensure to replace 'PORT_A' with the actual port number
#define LED_PIN GPIO_PIN(PORT_B, 0)   // Ensure to replace 'PORT_B' with the actual port number
#define DHT_TYPE DHT11
#define TEMP_THRESHOLD 30  // Temperature threshold for warning

static dht_t dht;

void read_sensor_and_toggle_led(void) {
    int16_t temp, hum;
    if (dht_read(&dht, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d, Humidity: %d\n", temp, hum);
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Sensor read failed\n");
    }
}

int main(void) {
    if (dht_init(&dht, &dht_params[DHT_TYPE][0]) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_sensor_and_toggle_led();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}