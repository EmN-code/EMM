#include <ztimer.h>
#include <stdio.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(PORT_X, X) // replace X with actual port and pin number
#define LED_PIN GPIO_PIN(PORT_Y, Y) // replace Y with actual port and pin number
#define TEMP_THRESHOLD 30          // temperature threshold in °C
#define TIMER_PERIOD (5U * US_PER_SEC) // 5 seconds period

static dht_t dht_dev;

static void dht_timer_cb(void *arg) {
    (void)arg; // suppress unused variable warning
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) != 0) {
        printf("Error reading DHT11 sensor\n");
        return;
    }
    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

    // Turn on LED if temperature exceeds threshold
    if (temp / 10 > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    // Initialize DHT sensor
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT_TYPE_DHT11;
    if (dht_init(&dht_dev, &dht_params) != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }
    
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a timer to read sensor every 5 seconds
    ztimer_periodic_t dht_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &dht_timer, dht_timer_cb, NULL, TIMER_PERIOD);
    ztimer_periodic_start(&dht_timer);

    // Main loop does nothing, timer handles sensor reading
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_PERIOD);
    }

    return 0;
}