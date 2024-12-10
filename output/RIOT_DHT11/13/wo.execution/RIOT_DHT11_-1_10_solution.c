#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds

static dht_t dev;

void* read_dht(void *arg) {
    (void)arg;
    int16_t temperature, humidity;

    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temperature / 10, temperature % 10,
                    humidity / 10, humidity % 10);
        }
        else {
            printf("Error reading DHT sensor\n");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht, NULL, "dht_reader");
    return 0;
}