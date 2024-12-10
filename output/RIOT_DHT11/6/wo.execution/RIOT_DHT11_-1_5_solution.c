#include "thread.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "nvs.h"
#include "thread_flags.h"
#include "mma8x5x_regs.h"
#include "nvs_flash.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x_params.h"

#define DHT_PIN GPIO2

void *dht_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT11, DHT_PIN) != 0) {
        printf("Error initializing DHT sensor\n");
        return NULL;
    }

    while (1) {
        // Read data from the DHT sensor
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        else {
            printf("Error reading from DHT sensor\n");
        }
        // Sleep for 5 seconds
        thread_sleep(5000);
    }
    return NULL;
}

int main(void) {
    // Start the DHT thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht");
    return 0;
}
