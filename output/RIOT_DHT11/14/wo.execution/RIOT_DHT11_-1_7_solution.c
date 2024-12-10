#include "thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"

// Define the GPIO pin for the DHT11 sensor
#define DHT11_PIN GPIO_PIN(0, 2)

// Define stack size for threads
#define THREAD_STACKSIZE_MAIN 1024

// Define the priority for the thread
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)

// Global variable to hold the thread stack
static char stack[THREAD_STACKSIZE_MAIN];

// Function to be executed by the created thread
void *dht_read_thread(void *arg) {
    (void)arg;
    dht_t dev;
    // Initialize the DHT11 sensor configuration
    dht_params_t params;
    params.pin = DHT11_PIN;
    params.type = DHT11;

    // Infinite loop to read the sensor every 5 seconds
    while (1) {
        int16_t temperature, humidity;

        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            // Print the read values
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Error reading DHT11 sensor\n");
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Create a thread for reading the DHT11 sensor
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  dht_read_thread, NULL, "dht_reader");

    // Let the main thread idle
    while (1) {
        thread_yield();
    }
    return 0;
}