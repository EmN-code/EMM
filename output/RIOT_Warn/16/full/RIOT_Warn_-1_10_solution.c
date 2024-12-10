#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h" // Assuming there is a required header for dht

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Correctly define the pin for the sensor
#define LED_PIN GPIO_PIN(PORT_B, 1)  // Correctly define the pin for the LED
#define THRESHOLD_TEMP 30            // Temperature threshold in Celsius
#define SLEEP_INTERVAL 5             // Interval between sensor reads in seconds

static dht_t dev;

void init(void) {
    int res;
    if ((res = dht_init(&dev, DHT_PIN, DHT11)) != 0) {
        printf("Error in DHT init: %d\n", res);
        return;
    }
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_write(LED_PIN, false);
}

void read_dht_and_control_led(void) {
    int16_t temp, hum;
    int res;

    while (1) {
        res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);

            if (temp > THRESHOLD_TEMP) {
                gpio_write(LED_PIN, true); // Turn on LED
            } else {
                gpio_write(LED_PIN, false); // Turn off LED
            }
        } else {
            printf("Error reading DHT: %d\n", res);
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    printf("Starting DHT11 sensor reading with LED warning system.\n");
    init();
    read_dht_and_control_led();
    return 0;
}