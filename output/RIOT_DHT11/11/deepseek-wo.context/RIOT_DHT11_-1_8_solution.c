
#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"
#include "fmt.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
