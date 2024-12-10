
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"
#include "event/thread.h"
#include "periph/gpio.h"

#define DHT_PIN             GPIO_PIN(0, 17)  // GPIO17 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define SLEEP_DURATION      5                // Sleep duration in seconds

static dht_t dht_dev;

void read_dht11(void)
{
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

int main(void)
{
    // Initialize the DHT11 sensor
    dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN);

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        xtimer_sleep(SLEEP_DURATION);
    }

    return 0;
}
