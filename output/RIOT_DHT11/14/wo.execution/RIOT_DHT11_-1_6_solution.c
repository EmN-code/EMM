#include "thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

// Define DHT parameters
#define DHT_PIN     GPIO_PIN(0, 2)
#define DHT_TYPE    DHT11

// Stack size for the thread
#define THREAD_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)

// Declare stack and ID for sensor reading thread
char sensor_stack[THREAD_STACKSIZE];
kernel_pid_t sensor_pid;

void *sensor_thread(void *arg) {
    (void)arg;

    // Initialize DHT device
    dht_t dev;
    dev.pin = DHT_PIN;
    dev.type = DHT_TYPE;
    
    int16_t temperature, humidity;

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read temperature/humidity\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    printf("Starting DHT11 reading thread\n");

    // Create a new thread for reading sensor data
    sensor_pid = thread_create(sensor_stack, sizeof(sensor_stack),
                                THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST,
                                sensor_thread, NULL, "sensor_thread");

    // Continue to handle MQTT or other main processes here

    return 0;
}