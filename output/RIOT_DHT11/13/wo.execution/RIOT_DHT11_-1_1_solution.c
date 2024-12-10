#include <stdio.h>
#include <dht.h>
#include <xtimer.h>

#define DHT_PIN GPIO2  // Define the GPIO pin where DHT11 is connected
#define INTERVAL (5U * 1000000U)  // 5 seconds interval in microseconds

static void *_dht_thread(void *arg) {
    dht_t dev;
    dht_params_t params;
    int16_t temp, hum;

    params.pin = DHT_PIN;
    params.type = DHT11;

    // Initialize DHT sensor
    if (dht_init(&dev, &params) != 0) {
        printf("Error initializing DHT sensor\n");
        return NULL;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT sensor\n");
        } else {
            // Print temperature and humidity
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Sleep for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Start the DHT thread
    thread_t dht_thread;
    char stack[THREAD_STACKSIZE_DEFAULT];

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _dht_thread, NULL, "dht_thread");

    // Main thread does nothing, it could sleep forever
    while(1) {
        xtimer_usleep(1000000);
    }

    return 0;
}