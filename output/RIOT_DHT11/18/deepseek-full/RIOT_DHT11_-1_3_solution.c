
#include "thread.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define SLEEP_TIME (5 * 1000 * 1000U)  // 5 seconds in microseconds

static void read_dht_task(void *arg)
{
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        if (dht_init(&dev, DHT_PIN, DHT_TYPE) == DHT_OK) {
            if (dht_read(&dev, &temp, &hum) == DHT_OK) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                       temp / 10, temp % 10, hum / 10, hum % 10);
            } else {
                puts("Failed to read from DHT sensor");
            }
        } else {
            puts("Failed to initialize DHT sensor");
        }
        xtimer_usleep(SLEEP_TIME);
    }
}

int main(void)
{
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht_task, NULL, "dht_reader");

    return 0;
}
