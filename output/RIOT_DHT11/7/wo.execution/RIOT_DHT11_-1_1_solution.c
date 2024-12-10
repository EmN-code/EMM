#include "thread_arch.h"
#include "dht.h"
#include "xtimer.h"
#include "ztimer.h"

// ESP32 GPIO for DHT11
#define DHT_PIN GPIO_PIN(0, 2)
// Timeout for DHT in microseconds
#define DHT_TIMEOUT (40 * US_PER_MS)

static dht_t dht_device;

void *read_dht(void *arg) {
    int16_t temp, hum;

    while (1) {        
        // Read temperature and humidity from DHT11
        if (dht_read(&dht_device, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    // Initialize the DHT11 sensor
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    
    if (dht_init(&dht_device, &params) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Create a thread to read data every 5 seconds
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  read_dht, NULL, "read_dht");

    return 0;
}