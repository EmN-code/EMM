#include "ztimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define THREAD_STACKSIZE_MAIN    (4096)
#define THREAD_PRIORITY_MAIN     (5)

#define DHT_PIN                  GPIO_PIN(0, 2)  // GPIO2 on ESP32

char stack[THREAD_STACKSIZE_MAIN];

void *dht_read_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  dht_read_thread, NULL, "dht_read_thread");
    return 0;
}