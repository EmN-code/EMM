
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "periph/gpio.h"

#define DHT_PIN            GPIO_PIN(0, 18)  // GPIO pin for DHT11
#define LED_PIN            GPIO_PIN(0, 2)   // GPIO pin for LED
#define TEMP_THRESHOLD     30               // Temperature threshold in Celsius
#define SLEEP_INTERVAL     (5 * 1000000U)   // 5 seconds in microseconds

static void read_dht(void *arg) {
    (void)arg;
    int16_t temp, hum;

    // Read temperature and humidity from DHT11
    if (dht_read(DHT_PIN, &temp, &hum) == 0) {
        printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);

        // Check if temperature exceeds the threshold
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn on the LED
        } else {
            gpio_clear(LED_PIN);  // Turn off the LED
        }
    } else {
        puts("Failed to read from DHT11 sensor");
    }

    // Schedule the next reading
    xtimer_sleep(SLEEP_INTERVAL / 1000000U);
    thread_yield();
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN_PU);  // Initialize DHT_PIN with pull-up resistor

    // Create an event thread for periodic DHT11 reading
    event_thread_create(NULL, "dht_reader", read_dht, NULL, 256, 0);

    return 0;
}
