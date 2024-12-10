#include "thread_arch.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include "MQTTClient.h"

#define DHT11_TYPE DHT11
#define GPIO_PIN 2

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN THREAD_PRIORITY_MAIN - 1

void *read_dht11(void *arg) {
    (void)arg;
    dht_t dev;

    // Initialize the DHT11 device
    if (dht_init(&dev, DHT11_TYPE, GPIO_PIN) != 0) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;

        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }

        // Sleep for 5 seconds
        thread_sleep(5);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];

    // Create a new thread to read from DHT11
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, read_dht11, NULL, "dht11_reader");

    // Let the main thread sleep indefinitely
    thread_sleep();

    return 0;
}