#include "thread_arch.h"
#include "thread.h"
#include "xtimer.h"
#include "dht.h"
#include "stdio.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 for board
#define DHT_TYPE DHT11

// Stack size for the reader thread
#define THREAD_STACKSIZE_MAIN (1024)

// Reader thread priority
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

static kernel_pid_t dht_thread_pid;
static char dht_stack[THREAD_STACKSIZE_MAIN];

void *dht_reading_loop(void *arg) {
    (void)arg;
    dht_t dev;
    dht_params_t params;

    // Initialize DHT driver
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT device\n");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;

        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Start the DHT reader thread
    dht_thread_pid = thread_create(
        dht_stack, sizeof(dht_stack), THREAD_PRIORITY_MAIN, 0,
        dht_reading_loop, NULL, "dht_reader_thread");

    return 0;
}