
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht_dev;
static int16_t temperature, humidity;

static void read_dht(void) {
    if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

static void dht_thread(void) {
    while (1) {
        read_dht();
        xtimer_usleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) == DHT_OK) {
        puts("DHT11 sensor initialized");
    } else {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht_thread");

    return 0;
}
