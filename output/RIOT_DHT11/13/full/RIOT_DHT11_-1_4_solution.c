#include "thread_flags.h"
#include "thread.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"

// DHT sensor configuration
#define DHT_PIN 2
#define DHT_TYPE DHT11

void *dht_reading_thread(void *arg) {
    (void) arg;

    // Initialize the DHT sensor structure
    dht_t dht;
    dht_params_t dht_params;
    int16_t temp, hum;

    // Set DHT type
    dht_params.type = DHT_TYPE;
    dht_params.pin = DHT_PIN;

    while (dht_init(&dht, &dht_params) != 0) {
        // Retry initialization if it fails
    }

    while (1) {
        if (dht_read(&dht, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Initialize NVS
    if (nvs_flash_init() != ESP_OK) {
        printf("Failed to initialize NVS\n");
        return -1;
    }

    // Create a thread for reading DHT sensor data
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_reading_thread, NULL, "dht_reading");

    // Main thread loop
    while (1) {
        thread_flags_wait(0);  // Wait forever
    }

    return 0;
}