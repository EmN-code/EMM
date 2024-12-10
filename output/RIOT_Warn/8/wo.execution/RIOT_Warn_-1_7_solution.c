// The following code demonstrates the use of an ESP32 board to read data from a DHT11 sensor every 5 seconds and control an LED based on the temperature reading.

// Import necessary headers for using peripherals, threading, and sensor communication
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "paho_mqtt.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "periph/rtc.h"

#include "ztimer.h" // Changed from xtimer_sleep to ztimer_sleep

#define DHT_PIN 0     // Specify the GPIO pin where the DHT11 is connected
#define LED_PIN 2     // Specify the GPIO pin where the LED is connected
#define TEMP_THRESHOLD 30 // Temperature threshold for LED warning

static char stack[THREAD_STACKSIZE_MAIN];

void *read_dht11(void *arg) {
    (void)arg;
    int16_t temp, hum;
    dht_t dev;

    while (1) {
        // Simulate the initialization and reading, since we are avoiding direct GPIO manipulation
        dht_read(&dev, &temp, &hum);
        printf("Temperature: %d, Humidity: %d\n", temp, hum);

        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN); // Turn on LED if temperature exceeds threshold
        } else {
            gpio_clear(LED_PIN); // Turn off LED otherwise
        }

        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds using ztimer
    }
    return NULL;
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    // Start DHT11 reading thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_dht11, NULL, "dht11_reader");

    // The main function will just idle as the worker thread does the task
    while (1) {}
    return 0;
}
