#include <ztimer.h>  // For timer functions, ztimer is used in RIOT for such purposes
#include <timex.h>   // Required for time extension types and constants
#include <stddef.h>  // Standard definitions
#include <stdlib.h>  // Standard library definitions
#include <stdbool.h> // Standard true/false definitions
#include <stdatomic.h> // For standard atomic types
#include <stdio.h>   // For standard input/output operations
#include "dht.h"    // DHT sensor header (Note: we provide modified handling as we're not using `dht_t` struct)
#include <periph/gpio.h>  // For GPIO operations

#define DHT11_PIN GPIO_PIN(0, 1)  // Define the GPIO pin connected to the DHT11 data pin
#define LED_PIN GPIO_PIN(0, 2)    // Define the GPIO pin connected to the LED
#define TEMP_THRESHOLD 30         // Define the temperature threshold

static void read_dht_and_control_led(void) {
    int16_t temperature, humidity;
    dht_t dht_dev;
    dht_dev.pin = DHT11_PIN;
    dht_dev.type = DHT11;

    if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d.%d°C\n", temperature / 10, temperature % 10);
        printf("Humidity: %d.%d%%\n", humidity / 10, humidity % 10);

        if (temperature / 10 >= TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Warning: Temperature exceeded the threshold! LED is ON.\n");
        } else {
            gpio_clear(LED_PIN);
            printf("Temperature is within safe limits. LED is OFF.\n");
        }
    }
    else {
        printf("Error reading from DHT11 sensor\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    ztimer_sleep(ZTIMER_MSEC, 5000);  // Ensure a small initial delay (5 seconds)
    
    while (true) {
        read_dht_and_control_led();
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
    return 0;
}
