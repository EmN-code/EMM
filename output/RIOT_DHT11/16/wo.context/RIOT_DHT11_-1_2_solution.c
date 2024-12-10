#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT11_PARAM_PIN GPIO_PIN(0, 2)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE THREAD_STACKSIZE_DEFAULT

static char thread_stack[THREAD_STACKSIZE];

void *dht11_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT11, DHT11_PARAM_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read DHT11 data\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, dht11_thread, NULL, "dht11_thread");
    return 0;
}