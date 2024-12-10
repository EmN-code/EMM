#include <stdio.h>
#include <stdlib.h>
#include <dht.h>
#include <xtimer.h>// include xtimer for timing functions
#include <thread.h>

#define DHT11_PIN 2 // GPIO pin where DHT11 is connected
#define INTERVAL 5  // Interval between reads in seconds

static char stack[THREAD_STACKSIZE_MAIN];

void *dht_sensor_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT11_PIN, DHT11) != DHT_OK) {
        printf("Could not initialize DHT sensor\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Error reading from sensor\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_sensor_thread, NULL, "dht11_reader");
    return 0;
}