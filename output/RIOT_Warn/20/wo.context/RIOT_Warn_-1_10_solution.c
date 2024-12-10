#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>  // Ensure you have this library available

#define DHT_PIN GPIO_PIN(PORT_A, 2)  // Use the appropriate GPIO port and pin
#define LED_PIN GPIO_PIN(PORT_B, 3)  // Use the appropriate GPIO port and pin
#define THRESHOLD_TEMP 30  // Temperature threshold in Celsius

static dht_t dev;

int main(void)
{
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read data from DHT sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    
    return 0;
}