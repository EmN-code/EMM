#include <thread_flags.h>
#include <nvs_flash.h>
#include <event/thread.h>
#include <dht_params.h>

#define DHT_PIN 2
#define SLEEP_INTERVAL_US (5 * 1000000) // 5 seconds
#define THREAD_STACKSIZE_MAIN (1 << 10)

void read_dht11(void *arg) {
    (void)arg;
    dht_t dev;
    dht_params_t dht_params = { .pin = DHT_PIN, .type = DHT_TYPE_DHT11 };
    while (1) {
        while (dht_init(&dev, &dht_params) != DHT_OK) {
            printf("DHT initialization failed, retrying...\n");
            usleep(SLEEP_INTERVAL_US); // Sleep for retry
        }
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11, retrying...\n");
        }
        usleep(SLEEP_INTERVAL_US); // Sleep for next read cycle
    }
}

int main(void) {
    nvs_flash_init();
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(read_dht11, stack, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, "dht11", NULL);
    while (1) {
        thread_flags_wait(0);
    }
    return 0;
}