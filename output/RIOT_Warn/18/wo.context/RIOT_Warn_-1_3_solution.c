#include <ztimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN  GPIO_PIN(PORT_X, X)  // define the GPIO pin for the DHT11
#define LED_PIN  GPIO_PIN(PORT_Y, Y)  // define the GPIO pin for the LED
#define THRESHOLD_TEMP 30              // set the temperature threshold
#define READ_INTERVAL (5 * US_PER_SEC) // set the read interval to 5 seconds

static dht_t dev;
atimer_t timer;

void read_sensor(void *arg) {
    (void)arg;
    int16_t temp, hum;

    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

        // Check temperature threshold
        if (temp / 10 > THRESHOLD_TEMP) {
            // Turn on LED
            gpio_set(LED_PIN);
        } else {
            // Turn off LED
            gpio_clear(LED_PIN);
        }
    } else {
        puts("Error reading DHT11 sensor");
    }

    // Reset the alarm
    ztimer_set(&timer, ztimer_now(ZTIMER_USEC) + READ_INTERVAL);
}

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        puts("Could not initialize DHT11 sensor");
        return 1;
    }

    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error initializing LED GPIO pin");
        return 1;
    }

    // Initialize alarm
    ztimer_set_callback(ZTIMER_USEC, &timer, read_sensor, NULL);
    ztimer_set(&timer, ztimer_now(ZTIMER_USEC) + READ_INTERVAL);

    // Loop endlessly
    while(1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}