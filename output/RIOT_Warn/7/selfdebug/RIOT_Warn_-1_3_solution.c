#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define LED_PIN GPIO_PIN(PORT_B, 2)
#define DHT_PIN GPIO_PIN(PORT_B, 3)
#define TEMP_THRESHOLD 30  // Temperature threshold in degree Celsius

void read_temp_and_control_led(void) {
    int16_t temp, hum;
    dht_params_t params;
    dht_t dev;

    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        puts("DHT initialization failed");
        return;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read from DHT sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_write(LED_PIN, 1);  // Turn the LED on
            } else {
                gpio_write(LED_PIN, 0);  // Turn the LED off
            }
        }

        xtimer_sleep(5);  // Wait for 5 seconds
    }
}

int main(void) {
    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error to initialize LED pin");
        return 1;
    }

    read_temp_and_control_led();
    return 0;
}