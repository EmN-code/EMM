#include "mma8x5x_params.h"
#include "dht_params.h"
#include "dht.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#include "xtimer.h"
#include "msg.h"

#define DHT_PIN GPIO_PIN(0, 2) // Define the pin connected to DHT11

gpio_t pin = DHT_PIN;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(DHT_TYPE_DHT11, &pin, &temp, &hum) != DHT_OK) {
        printf("Failed to read from DHT sensor\n");
        return;
    }
    // Convert to typical values for display/logging
    temp /= 10;
    hum /= 10;

    printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
}

int main(void) {
    // Initialize the DHT sensor pin
    if (gpio_init(pin, GPIO_IN) < 0) {
        printf("Error to init GPIO\n");
        return 1;
    }
    printf("DHT sensor initialized on GPIO %d\n", DHT_PIN);

    // Continuous loop to read sensor data every 5 seconds
    while (1) {
        read_dht11();
        xtimer_sleep(5);
    }

    return 0;
}