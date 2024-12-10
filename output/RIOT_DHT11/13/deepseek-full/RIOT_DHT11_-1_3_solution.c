
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static void read_dht11(void)
{
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("DHT11 read failed");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }
}

int main(void)
{
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht11, NULL, "dht11_reader");

    return 0;
}
