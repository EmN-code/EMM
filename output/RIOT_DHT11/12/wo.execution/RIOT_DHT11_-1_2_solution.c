#include "thread_arch.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"

#define GPIO_PIN 2
#define SLEEP_DURATION 5

static char stack[THREAD_STACKSIZE_MAIN];

void *read_dht11(void *arg) {
    dht_t dev;

    if (dht_init(&dev, GPIO_PIN) != DHT_OK) {
        return NULL;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        else {
            printf("Failed to read from DHT11\n");
        }
        thread_sleep_s(SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_dht11, NULL, "dht_reader");
    return 0;
}