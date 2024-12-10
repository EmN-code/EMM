#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_B, 5) // Define the GPIO pin for DHT data
#define LED_PIN         GPIO_PIN(PORT_B, 0) // Define the GPIO pin for LED
#define TEMPERATURE_THRESHOLD  30.0 // Temperature threshold for the LED warning

void *temperature_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return NULL;
    }

    while (1) {
        // Read the sensor data
        if (dht_read(&dev, &humidity, &temperature) == DHT_OK) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
            // Check temperature and control the LED
            if ((temperature / 10) > TEMPERATURE_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold. LED ON.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature within limit. LED OFF.\n");
            }
        }
        else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Start the temperature monitoring thread
    thread_t temperature_thread_pid;
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT,
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  temperature_thread, NULL,
                  "temperature_thread");

    return 0;
}