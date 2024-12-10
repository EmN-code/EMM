import "string.h"
import "mma8x5x_params.h"
import "dht_params.h"
import "thread.h"
import "dht.h"
import "mma8x5x.h"
import "MQTTClient.h"
import "paho_mqtt.h"
import "mma8x5x_regs.h"
import "event/thread.h"
import "periph/rtc.h"
import "periph/gpio.h"
import "periph/i2c.h"

#include <stdio.h>
#include <stdlib.h>

// Macros for thresholds and GPIO definitions
#define TEMP_THRESHOLD 30  // Temperature threshold
#define GPIO_LED 5          // GPIO pin for LED
#define DHT_PIN 4           // GPIO pin connected to DHT sensor

// Function prototype
void *read_dht_and_control_led(void *arg);

// The main function initializes the sensor and creates a thread to read the sensor data
int main(void) {
    kernel_pid_t thread_pid;
    void *thread_stack = (void *)malloc(THREAD_STACKSIZE_DEFAULT);
    if (thread_stack == NULL) {
        printf("Failed to allocate memory for thread stack\n");
        return 1;
    }

    thread_pid = thread_create((char *)thread_stack, THREAD_STACKSIZE_DEFAULT,
                               THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                               read_dht_and_control_led, NULL, "DHTReader");
    if (thread_pid == -1) {
        printf("Failed to create thread\n");
        free(thread_stack);
        return 1;
    }

    while (1) {
        // Main thread doing other tasks
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}

// Read DHT sensor and control LED
void *read_dht_and_control_led(void *arg) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize DHT sensor
    dht_init(&dev, DHT_PIN, DHT11);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // Control LED based on the temperature
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(GPIO_LED);
            } else {
                gpio_clear(GPIO_LED);
            }
        }
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}