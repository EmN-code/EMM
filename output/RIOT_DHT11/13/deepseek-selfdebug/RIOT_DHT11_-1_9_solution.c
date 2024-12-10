
#include "periph/gpio.h"
#include "periph/timer.h"
#include "dht.h"
#include "xtimer.h"
#include "fmt.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Initialization of DHT11 sensor failed");
        return 1;
    }

    // Set up a periodic timer to read the DHT11 sensor every 5 seconds
    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_set(&timer, INTERVAL, (void (*)(void *))read_dht, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
