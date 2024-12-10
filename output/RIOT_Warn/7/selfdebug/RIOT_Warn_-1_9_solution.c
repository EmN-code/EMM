#include <xtimer.h>
#include <mutex.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN         GPIO_PIN(PORT_A, 0)  // Adjust according to the actual port definition in RIOT
#define LED_PIN         GPIO_PIN(PORT_B, 1)  // Adjust the GPIO pin
#define TEMP_THRESHOLD  30  // Temperature threshold in °C

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(&dht, &DHT_TYPE_DHT11) != DHT_SUCCESS) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temperature, humidity;
        if (dht_measure(&dht, &temperature, &humidity) != DHT_SUCCESS) {
            printf("Failed to read data from sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}