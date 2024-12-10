#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "dht.h"

// Define macro for checking null pointers
#define CHECK_NULL(ptr)                                      \
    if ((ptr) == NULL) {                                     \
        puts("Error: null pointer encountered");             \
        return 1;                                            \
    }

#define TEMPERATURE_THRESHOLD 30  // Define the temperature threshold
#define DHT_PIN GPIO_PIN(PORT_A, 5)  // Pin connected to DHT
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Pin connected to LED

// Function to connect and read DHT data
float read_dht_temperature(void) {
    float temperature = 0.0f;
    int result;

    // Initialize the DHT sensor
    while ((result = dht_init(DHT_PIN, DHT11)) != 0) {
        printf("DHT init failed with %d. Retrying...\n", result);
        xtimer_sleep(1);
    }

    // Read temperature until successful
    while ((result = dht_read(DHT_PIN, &temperature, DHT_READ_TEMP)) != 0) {
        printf("Failed to read temperature: %d\n", result);
        xtimer_sleep(1);
    }

    return temperature;
}

// Task function for reading the DHT sensor
void *dht_task(void *arg) {
    (void)arg;
    float temperature;

    while (1) {
        temperature = read_dht_temperature();
        printf("Current temperature: %.1f°C\n", temperature);

        if (temperature > TEMPERATURE_THRESHOLD) {
            printf("Temperature exceeded threshold! Turning on LED.\n");
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Initialize GPIO
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error: Unable to initialize LED GPIO");
        return 1;
    }

    // Create thread for the DHT task
    char stack_thread[THREAD_STACKSIZE_MAIN];
    thread_create(stack_thread, sizeof(stack_thread), THREAD_PRIORITY_MAIN, 0,
                  dht_task, NULL, "dht_task");

    // Main loop does nothing, job is handled by threads
    while (1) {
        thread_yield();
        xtimer_sleep(1);
    }

    return 0;
}