#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(0, 5) // Assuming the DHT11 is connected on GPIO Port 0 Pin 5
#define LED_PIN         GPIO_PIN(0, 6) // Assuming the LED is connected on GPIO Port 0 Pin 6
#define TEMP_THRESHOLD  25             // Temperature threshold in °C
#define DELAY           (5U * US_PER_SEC)  // 5 seconds delay

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT11 sensor
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE_DHT11;
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity from DHT11
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            // Turn on LED if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Wait for 5 seconds
        xtimer_sleep(DELAY);
    }
    return 0;
}