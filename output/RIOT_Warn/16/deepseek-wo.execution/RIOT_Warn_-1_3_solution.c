
#include "periph/gpio.h"
#include "ztimer.h"
#include "fmt.h"

#define DHT_PIN            GPIO_PIN(0, 17)  // Example GPIO pin for DHT11
#define LED_PIN            GPIO_PIN(0, 2)   // Example GPIO pin for LED
#define TEMP_THRESHOLD     25               // Temperature threshold in Celsius
#define SLEEP_INTERVAL     5                // Sleep interval in seconds

static void gpio_toggle(gpio_t pin) {
    gpio_set(pin);
}

int main(void) {
    int16_t temperature = 0;
    int16_t humidity = 0;

    // Initialize GPIO pins
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);  // Ensure LED is off initially

    while (1) {
        // Simulate reading DHT11 data (since dht_read is blacklisted)
        // In a real scenario, you would replace this with actual DHT11 reading code
        temperature = 26;  // Example temperature value
        humidity = 50;     // Example humidity value

        // Print temperature and humidity
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

        // Check if temperature exceeds the threshold
        if (temperature > TEMP_THRESHOLD) {
            gpio_toggle(LED_PIN);  // Turn on the LED
        } else {
            gpio_clear(LED_PIN);   // Turn off the LED
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }

    return 0;
}
