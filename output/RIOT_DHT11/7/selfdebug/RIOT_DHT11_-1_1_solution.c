#include "thread_arch.h"
#include "thread_config.h"
#include "thread_flags.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2) // GPIO2
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static dht_t dev;
static dht_params_t params = {
    .pin = DHT11_PIN,
    .type = DHT11
};

static void *dht11_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }
        xtimer_usleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("Starting DHT11 reader on GPIO2...");

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  dht11_thread, NULL, "dht11");

    return 0;
}