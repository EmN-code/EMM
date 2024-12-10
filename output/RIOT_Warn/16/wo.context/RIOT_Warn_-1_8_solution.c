#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Define proper GPIO pin for the LED
#define DHT_PIN GPIO_PIN(PORT_B, 3)  // Define proper GPIO pin for the DHT

#define THRESHOLD_TEMP 30  // Temperature threshold

int main(void)
{
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    dht_t dev;
    int16_t temp, hum;
    
    // Initialize DHT sensor (assuming it takes port and pin number)
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("DHT initialization failed\n");
        return -1;
    }
    
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp / 10, hum / 10);

            // Check if temperature exceeds the threshold
            if (temp / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);  // Turn on LED
            }
            else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }
        else {
            printf("Failed to read from DHT sensor\n");
        }
        
        xtimer_sleep(5);  // Wait for 5 seconds
    }

    return 0;
}