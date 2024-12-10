#include <xtimer.h>  // Include for timer functionality
#include <stdio.h>    // Include for standard input/output
#include <stdlib.h>   // Include for standard library
#include <string.h>   // Include for string handling
#include "dht.h"    // Include DHT sensor header
#include "periph/gpio.h" // Include GPIO peripheral header

#define DHT11_DATA_PIN GPIO_PIN(PORT_A, 0) // Define the DHT11 data pin
#define LED_PIN GPIO_PIN(PORT_B, 1)        // Define the LED pin
#define TEMP_THRESHOLD 30                  // Temperature threshold in Celsius
#define INTERVAL (5U * US_PER_SEC)         // 5 seconds interval

static dht_t dev;
static int16_t temperature, humidity;

void read_dht11(void) {
    if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
        printf("Error reading DHT11\n");
    }
}

void control_led_based_on_temperature(void) {
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
        printf("Temperature %d exceeds threshold! LED ON.\n", temperature);
    }
    else {
        gpio_clear(LED_PIN);
        printf("Temperature %d below threshold. LED OFF.\n", temperature);
    }
}

int main(void) {
    // Initialize GPIO for LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: unable to initialize LED_PIN\n");
    }

    // Initialize DHT11
    if (dht_init(&dev, DHT11_DATA_PIN) != DHT_OK) {
        printf("Error: unable to initialize DHT11_DATA_PIN\n");
    }

    // Main loop
    while (1) {
        read_dht11(); // Read the sensor
        control_led_based_on_temperature(); // Control LED

        xtimer_usleep(INTERVAL); // Sleep for 5 seconds
    }
    return 0;
}