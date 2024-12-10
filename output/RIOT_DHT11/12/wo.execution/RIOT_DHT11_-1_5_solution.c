#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "thread_arch.h"
#include "thread.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "architecture.h"

#define DHT_PIN 2

void *dht_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("DHT init failed\n");
        return NULL;
    }
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            printf("Failed to read from sensor\n");
        }
        thread_sleep_s(5);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];

    // Create a thread to handle DHT reading
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht_thread");

    // Prevent main from exiting
    while (1) {
        thread_sleep_s(1);
    }
    return 0;
}
