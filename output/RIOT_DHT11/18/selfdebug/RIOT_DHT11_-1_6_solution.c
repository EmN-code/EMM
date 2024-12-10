#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <thread_config.h>
#include <thread.h>
#include <periph_gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * 1000000U) // Interval to read sensor every 5 seconds

static dht_params_t dht_params;
static dht_t dev;
static kernel_pid_t thread_pid;
static char stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    (void)arg;  // Unused parameter

    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        xtimer_sleep(INTERVAL / 1000000U);
    }

    return NULL;
}

int main(void) {
    // Set parameters for DHT sensor
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;

    // Initialize DHT sensor
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Create a new thread to read the DHT sensor
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");
    if (thread_pid < 0) {
        printf("Error creating thread\n");
        return 1;
    }

    // Main function ends, but the thread continues
    return 0;
}