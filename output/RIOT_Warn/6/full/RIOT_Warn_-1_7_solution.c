#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

#define TEMP_THRESHOLD 30 // Set the temperature threshold
#define GPIO_PIN 2       // Define the GPIO pin where the LED is connected

// Initialize thread parameters
static char thread_stack[THREAD_STACKSIZE_MAIN];

void *temperature_reading_thread(void *arg) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the DHT11 device
    if (dht_init(&dev, &(dht_params[0])) != ESP_OK) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != ESP_OK) {
            printf("Failed to read from DHT11\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            // Check if temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(GPIO_PIN);
            } else {
                gpio_clear(GPIO_PIN);
            }
        }
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    // Initialize GPIO
    if (gpio_init(GPIO_PIN, GPIO_OUT) != ESP_OK) {
        printf("Failed to initialize GPIO\n");
        return -1;
    }

    // Setup and run the temperature reading thread
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 
                  THREAD_CREATE_STACKTEST, temperature_reading_thread, NULL, "Temperature Thread");

    return 0;
}