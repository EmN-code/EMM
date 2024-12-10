#include "mqttclient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"
#include "thread_arch.h"
#include "thread.h"
#include "event/thread.h"

#define GPIO_PIN 2
#define STACKSIZE THREAD_STACKSIZE_DEFAULT
#define PRIORITY THREAD_PRIORITY_MAIN - 1

static char sensor_thread_stack[STACKSIZE];

typedef struct {
    int16_t temperature;
    int16_t humidity;
} sensor_data_t;

void* sensor_thread(void *arg) {
    dht_t dev;
    int16_t temperature, humidity;
    sensor_data_t *data = (sensor_data_t*)arg;

    // Initialize DHT sensor
    dht_init(&dev, DHT11, GPIO_PIN);

    while (1) {
        // Read the temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            data->temperature = temperature;
            data->humidity = humidity;
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Error reading DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    sensor_data_t sensor_data;
    thread_create(sensor_thread_stack, STACKSIZE, PRIORITY, 0, sensor_thread, &sensor_data, "sensor thread");
    while (1) {
        thread_sleep();
    }
    return 0;
}