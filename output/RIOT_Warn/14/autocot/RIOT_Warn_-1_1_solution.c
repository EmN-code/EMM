#include <xtimer.h>  
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // example pin, adjust as needed
#define LED_PIN GPIO_PIN(PORT_B, 0)  // example pin, adjust as needed
#define TEMP_THRESHOLD 30   // example temperature threshold
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT device
    if (dht_init(&dev, &DHT_PIN) != 0) {
       puts("Error initializing DHT sensor\n");
       return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // read temperature and humidity from sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d th humidity: %d %%\n", temp, hum);

            // Check if the temperature exceeds the threshold
            if (temp >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Sleep for defined interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}