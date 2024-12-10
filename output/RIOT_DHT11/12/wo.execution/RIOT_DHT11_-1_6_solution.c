#include "thread_arch.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"

#include "periph/gpio.h"
#include "xtimer.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define STACKSIZE THREAD_STACKSIZE_MAIN

void *dht_thread(void *arg) {
    (void)arg;  // Unused parameter
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    dht_init(&dev, &params);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    char stack[STACKSIZE];
    kernel_pid_t dht_pid;

    // Create a new thread to handle DHT reading
    dht_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                            CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    if (dht_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create DHT thread\n");
        return 1;
    }

    return 0;
}