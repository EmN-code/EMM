#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN         GPIO_PIN(0, 5)  // Use appropriate pin number
#define TEMP_THRESHOLD  30  // Temperature threshold
#define LED_PIN         GPIO_PIN(0, 6)  // Use appropriate pin number for LED

// Initialize DHT sensor
static dht_t dev;
static dht_params_t params = {
    .pin = DHT_PIN,
    .type = DHT11
};

void read_sensor_and_trigger_led(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp > TEMP_THRESHOLD * 10) {  // Convert to deci-degrees
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    }
    else {
        printf("Error reading DHT sensor\n");
    }
}

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dev, &params) != 0) {
        printf("Could not initialize DHT sensor\n");
        return -1;
    }
    
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Loop and read DHT sensor every 5 seconds
    while (1) {
        read_sensor_and_trigger_led();
        xtimer_sleep(5);
    }
    return 0;
}