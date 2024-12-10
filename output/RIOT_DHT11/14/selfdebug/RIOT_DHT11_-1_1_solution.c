#include <timex.h>
#include <thread.h>
#include "dht.h"
#include "esp_idf_gpio.h"

#define DHT_PIN GPIO_NUM_2  // Define the GPIO pin for DHT11
#define SLEEP_TIME (5 * US_PER_SEC)  // 5 seconds sleep time converted to microseconds

static dht_t dht;
static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT11
};

void *dht_reading_thread(void *arg) {
    int16_t temperature, humidity;

    while (1) {
        // Read the DHT11 sensor
        if (dht_read(&dht, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %2.1f°C, Humidity: %2.1f%%\n", temperature / 10.0, humidity / 10.0);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_TIME);
    }
    return NULL;
}

int main(void) {
    // Initialize the DHT11 sensor
    dht_init(&dht, &dht_params);

    // Create a thread that will perform DHT11 readings
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, dht_reading_thread, NULL, "dht_reader");

    return 0;
}