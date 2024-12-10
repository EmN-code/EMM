#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 5) // Assuming the DHT is connected to GPIO Port 0, Pin 5
#define LED_PIN GPIO_PIN(0, 7) // Assuming the LED is connected to GPIO Port 0, Pin 7
#define THRESHOLD_TEMP 30.0    // Temperature threshold for LED warning
#define INTERVAL 5 * US_PER_SEC // 5 seconds interval

static dht_t dev;

dht_t *dht_init(gpio_t pin)
{
    gpio_init(pin, GPIO_IN);
    return &dev;
}

void read_dht(void)
{
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d, Humidity: %d\n", temp, hum);
        if (temp > THRESHOLD_TEMP * 10) { // DHT returns temperature in °C * 10
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}

int main(void) {
    // Initialize DHT sensor
    dht_init(DHT_PIN);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht();
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return 0;
}