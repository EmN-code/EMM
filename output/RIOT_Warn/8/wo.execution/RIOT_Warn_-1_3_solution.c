// Import necessary libraries
#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "periph/gpio.h"
#include "dht_params.h"
#include "dht.h"

// Define necessary constants and variables
#define DHT_PIN     GPIO_PIN(PORT_A, 0)
#define LED_PIN     GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30

void *dht_thread(void *arg) {
    (void)arg; // Avoid unused parameter warning
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize DHT sensor
    if (dht_init(&dev, &dht_params[0]) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return NULL;
    }

    // Read the sensor data every 5 seconds
    while (1) {
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on the LED
            } else {
                gpio_clear(LED_PIN); // Turn off the LED
            }
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create thread for DHT sensor reading
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht_reader");

    return 0;
}
