
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

static void read_dht11(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return;
    }

    // Read temperature and humidity
    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        puts("DHT11 read failed");
        return;
    }

    // Print temperature and humidity
    printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
           temp / 10, temp % 10, hum / 10, hum % 10);

    // Check if temperature exceeds the threshold
    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);  // Turn on the LED
    } else {
        gpio_clear(LED_PIN);  // Turn off the LED
    }
}

int main(void) {
    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
