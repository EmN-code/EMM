#include <stdio.h>         // For input/output functions
#include <stdlib.h>        // For standard library functions
#include "timex.h"      // Timer and time functions specific to RIOT
#include "thread.h"    // For thread handling in RIOT
#include "periph/gpio.h"  // GPIO control
#include "dht.h"       // DHT11 sensor handling

#define DHT11_PIN GPIO_PIN(0, 5) // Define which pin the DHT11 is connected to
#define LED_PIN GPIO_PIN(0, 18) // Define which pin the LED is connected to
#define TEMP_THRESHOLD 30  // Define threshold temperature in Celsius

int main(void) {
    // Initialize GPIO pins
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    if (dht_init(&dev, DHT11, DHT11_PIN) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;  // Variables to store temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn ON the LED
            } else {
                gpio_clear(LED_PIN);  // Turn OFF the LED
            }
        }
        xtimer_sleep(5);  // Wait for 5 seconds
    }
    return 0;
}