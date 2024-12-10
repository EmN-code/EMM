
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define DHT_TYPE DHT11

static dht_t dht_dev;

void read_dht_sensor(void)
{
    int16_t temperature, humidity;
    if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void)
{
    // Initialize the DHT sensor
    if (dht_init(&dht_dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        puts("Initialization of DHT sensor failed");
        return 1;
    }

    // Set up a periodic timer to read the sensor every 5 seconds
    while (1) {
        read_dht_sensor();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
