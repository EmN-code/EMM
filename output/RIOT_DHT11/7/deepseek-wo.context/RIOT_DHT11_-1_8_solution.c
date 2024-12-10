
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "fmt.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dev;

int main(void)
{
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read from DHT11 sensor");
        } else {
            char temp_str[10];
            char hum_str[10];
            fmt_s16_dfp(temp_str, temp, -1);
            fmt_s16_dfp(hum_str, hum, -1);
            printf("Temperature: %s°C, Humidity: %s%%\n", temp_str, hum_str);
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
