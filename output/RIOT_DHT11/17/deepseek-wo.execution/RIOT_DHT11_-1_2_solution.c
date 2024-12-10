
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000UL) // 5 seconds in microseconds

static void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(DHT_TYPE_DHT11, DHT11_PIN, &hum, &temp) == DHT_OK) {
        // Process the temperature and humidity values
        // For example, you can log or print the values here
    } else {
        // Handle error
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(DHT_TYPE_DHT11, DHT11_PIN) != DHT_OK) {
        // Handle initialization error
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
