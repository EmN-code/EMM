# Include necessary header files in alphabetical order
#include "xtimer.h"   // For timing functionalities
#include "thread.h"   // For multithreading support
#include "timex.h"    // For time conversion functions
#include "periph/gpio.h"  // For GPIO operations
#include "dht.h"      // For DHT sensor operations

#define DHT_PIN GPIO_PIN(0, 4) // Define the data pin for the DHT sensor
#define LED_PIN GPIO_PIN(0, 2) // Define the pin for the LED
#define TEMP_THRESHOLD 30      // Set the temperature threshold for the warning in Celsius
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5-second interval for reading sensor

static dht_t dev;

void *dht_reader_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading DHT11 sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize the GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the DHT sensor
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Create a separate thread for reading the DHT sensor
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_reader_thread, NULL, "dht_reader");

    // Keep the main thread active
    thread_sleep();
    return 0;
}