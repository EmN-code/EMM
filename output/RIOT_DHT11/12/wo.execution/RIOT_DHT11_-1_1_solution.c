#include <thread_arch.h>  // Including this first as per alphabetical order
#include <thread.h>
#include <dht.h>
#include <stdio.h>

#define DHT_PIN 2  // GPIO number to which DHT11 is connected
#define SLEEP_TIME 5  // Time in seconds between measurements

static char stack[THREAD_STACKSIZE_MAIN];  // Stack for the thread

void *dht_thread(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    dht_t dev;
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return NULL;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for the defined interval
        thread_sleep_s(SLEEP_TIME);
    }

    return NULL;
}

int main(void) {
    // Create a thread to read DHT sensor data
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  dht_thread, NULL, "dht");

    return 0;
}