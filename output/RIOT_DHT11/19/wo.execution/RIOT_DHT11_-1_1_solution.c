#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define INTERVAL (5U * US_PER_SEC)

static dht_t dht_dev;

void *read_dht(void *arg) {
    (void)arg; // To avoid unused parameter warning
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, abs(temp % 10), hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("DHT sensor reader example\n");

    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT_TYPE;

    // Initialize the DHT sensor
    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("Failed to initialize DHT device\n");
        return 1;
    }

    // Create a thread to read the sensor
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 
                  THREAD_CREATE_STACKTEST, read_dht, NULL, "read_dht");

    return 0;
}