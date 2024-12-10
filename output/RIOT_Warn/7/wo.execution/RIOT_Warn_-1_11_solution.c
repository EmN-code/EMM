#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>
#include <stdlib.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30 // temperature threshold in degree C
#define DHT_TYPE DHT11

dht_t dev;

// Mutex to protect shared resources
static mutex_t lock;

void *read_sensor(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        mutex_lock(&lock);
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        }
        mutex_unlock(&lock);

        // Check temperature and control LED
        if (temperature / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); // sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize the mutex
    mutex_init(&lock);
    
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT11 sensor
    dht_params_t params;
    params.type = DHT_TYPE;
    params.pin = DHT11_PIN;
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize sensor\n");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_sensor, NULL, "sensor_thread");

    // Main thread can perform other tasks or be idle
    while (1) {
        thread_yield();
    }

    return 0;
}