
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "periph/gpio.h"

#define DHT_PIN            GPIO_PIN(0, 17)  // GPIO pin for DHT11
#define LED_PIN            GPIO_PIN(0, 2)   // GPIO pin for LED
#define TEMP_THRESHOLD     30               // Temperature threshold in Celsius
#define SLEEP_INTERVAL     (5 * US_PER_SEC) // 5 seconds

static void read_dht(void *arg)
{
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
    xtimer_sleep(SLEEP_INTERVAL);
    thread_yield();
}

int main(void)
{
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT11 sensor
    if (dht_init(DHT_PIN) != 0) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    // Create an event thread for periodic DHT11 reading
    event_thread_create("dht_reader", 1024, THREAD_PRIORITY_MAIN - 1, 0, read_dht, NULL);

    return 0;
}
