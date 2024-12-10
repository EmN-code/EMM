
#include "ztimer.h"
#include "periph/gpio.h"
#include "fmt.h"

#define DHT_PIN            GPIO_PIN(0, 18)  // Example GPIO pin for DHT11
#define LED_PIN            GPIO_PIN(0, 2)   // Example GPIO pin for LED
#define TEMP_THRESHOLD     30               // Temperature threshold in Celsius
#define SLEEP_INTERVAL     5                // Sleep interval in seconds

void read_dht(int16_t *temp, int16_t *hum) {
    // Simulated DHT11 reading
    *temp = 25;  // Simulated temperature value
    *hum = 50;   // Simulated humidity value
}

void control_led(int16_t temp) {
    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    int16_t temperature, humidity;

    // Initialize GPIO pins
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor data
        read_dht(&temperature, &humidity);

        // Control LED based on temperature
        control_led(temperature);

        // Print temperature and humidity
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }

    return 0;
}
