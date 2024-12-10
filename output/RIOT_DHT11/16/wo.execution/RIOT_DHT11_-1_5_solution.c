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

#define SENSOR_THREAD_STACKSIZE (KERNEL_CONF_STACKSIZE_DEFAULT)
#define SENSOR_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char sensor_thread_stack[SENSOR_THREAD_STACKSIZE];

void *sensor_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    dev.pin = GPIO_PIN(0, 2); // Port 0, Pin 2
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }
        else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_usleep(5000000); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    printf("Starting DHT11 sensor thread\n");
    thread_create(sensor_thread_stack, sizeof(sensor_thread_stack),
                  SENSOR_THREAD_PRIORITY, 0, sensor_thread, NULL, "sensor_thread");
    return 0;
}