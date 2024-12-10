#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <thread.h>
#include <dht.h>
#include <periph/gpio.h>
#include <MQTTClient.h>

#define DHT_PIN GPIO_PIN(0, 2)  // Use GPIO 2 for DHT11
#define SLEEP_INTERVAL (5U * 1000000U) // Sleep for 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];

void *read_dht(void *arg) {
    (void) arg;
    dht_t dev;
    int16_t temp, hum;
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("DHT initialization failed\n");
        return NULL;
    }
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("RIOT DHT11 Reader");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, read_dht, NULL, "dht_reader");
    return 0;
}