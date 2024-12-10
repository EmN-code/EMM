#include <xtimer.h>
#include <kernel_defines.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Assuming GPIO port A, pin 0
#define LED_PIN GPIO_PIN(PORT_A, 1) // Assuming GPIO port A, pin 1
#define TEMP_THRESHOLD 30
#define SLEEP_TIME 5 * US_PER_SEC

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(DHT_PIN, DHT_TYPE_DHT11) != DHT_OK) {
        puts("Error initializing DHT11 sensor");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        // Read data from the DHT sensor
        int res = dht_read(&temp, &hum, DHT_PIN, DHT_TYPE_DHT11);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Compare and set LED state
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Error reading DHT11 sensor");
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_TIME / US_PER_MS);
    }
    
    return 0;
}