#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PA, 6)  // Assuming PA_6 is the pin connected to the DHT11 data pin
#define LED_PIN GPIO_PIN(PB, 7)  // Assuming PB_7 is the pin connected to the LED
#define TEMP_THRESHOLD 30        // Temperature threshold for warning in Celsius

static dht_t dev;

void read_and_check_temp(void)
{
    int16_t temperature, humidity;
    if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
        puts("Failed to read from DHT sensor!");
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
    
    // Check if temperature exceeds threshold
    if (temperature / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void)
{
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor!");
        return 1;
    }
    
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Failed to initialize LED pin!");
        return 1;
    }
    
    // Create a timer to read sensor value every 5 seconds
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        read_and_check_temp();
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
    }
    return 0;
}