#include "thread.h"
#include "thread_arch.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"

#define DHT_PIN  2  // Define the GPIO pin connected to DHT11
#define DHT_TYPE DHT11
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)

char stack[THREAD_STACKSIZE_MAIN];

void *dht_read_thread(void *arg) {
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;
    dht_init(&dev, &params);
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        thread_sleep_s(5);  // Wait for 5 seconds
    }
    return NULL;
}

int main(void) {
    kernel_pid_t dht_thread = thread_create(stack, sizeof(stack),
                                             THREAD_PRIORITY_MAIN - 1,
                                             THREAD_CREATE_STACKTEST,
                                             dht_read_thread,
                                             NULL, "dht_read_thread");

    if (dht_thread < 0) {
        printf("Error: Thread creation failed\n");
    }

    return 0;
}