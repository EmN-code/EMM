#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define DHT_PIN         GPIO_PIN(PORT_A, 5) // Correct pin according to your setup
#define LED_PIN         GPIO_PIN(PORT_A, 6) // Correct pin according to your setup
#define TEMP_THRESHOLD  30 // Temperature threshold for LED to turn on
#define DELAY           (5U * MS_PER_SEC) // Delay for 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Error initializing DHT sensor!");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Error reading from DHT sensor!");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
            if (temp/10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        xtimer_sleep(5);
    }

    return 0;
}