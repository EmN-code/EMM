
#include "periph/i2c.h"
#include "thread.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define DHT_TYPE            DHT11           // DHT11 sensor
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds in microseconds

static dht_t dht_dev;

static void read_dht(void)
{
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

static void *dht_thread(void *arg)
{
    (void)arg;
    while (1) {
        read_dht();
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void)
{
    if (dht_init(&dht_dev, DHT_TYPE, DHT_PIN) == DHT_OK) {
        thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                      0, dht_thread, NULL, "dht_thread");
    } else {
        puts("Failed to initialize DHT sensor");
    }

    return 0;
}
