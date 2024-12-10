#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"

#define DHT11_PIN GPIO_PIN(0, 2)  // Using GPIO 2 on Port 0
#define THREAD_STACKSIZE  (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)

void *read_dht(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor structure
    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0, read_dht, NULL, "DHT_Thread");
    return 0;
}