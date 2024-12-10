#include "xtimer.h"
#include "periph/gpio.h"
#include <stdio.h>
#include <inttypes.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // Adjust to your specific pin
#define DHT11_TYPE DHT11  // Ensure DHT11 is defined in your dht library
#define TEMP_THRESHOLD 30 // Temperature threshold for turning on the LED

static dht_t dev;
static gpio_t led = GPIO_PIN(PORT_B, 0); // Define your LED pin

int read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) != DHT11_OK) {
        printf("Error reading DHT11\n");
        return -1;
    }

    printf("Temperature: %" PRId16 " Humidity: %" PRId16 "\n", temp, hum);
    if (temp > TEMP_THRESHOLD) {
        gpio_set(led);
    } else {
        gpio_clear(led);
    }
    return 0;
}

int main(void) {
    dev.params.pin = DHT11_PIN;
    dev.params.type = DHT11_TYPE;

    dht_init(&dev); // Initialize the DHT device

    gpio_init(led, GPIO_OUT);

    while (1) {
        read_dht11();
        xtimer_sleep(5); 
    }
    return 0;
}
