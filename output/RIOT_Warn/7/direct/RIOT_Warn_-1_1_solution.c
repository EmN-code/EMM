#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 5)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30.0
#define READ_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

void *dht11_read(void *arg) {
    dht_t dev;
    int16_t temp, hum;
    
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Error: Unable to initialize DHT11 sensor\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error: Unable to read data from DHT11 sensor\n");
        } else {
            float temp_c = temp / 10.0;
            printf("Temperature: %.1f°C, Humidity: %d%%\n", temp_c, hum);
            if (temp_c > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeds threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_usleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("Starting DHT11 sensor monitoring\n");

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread to handle the DHT11 sensor reading
    char dht11_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(dht11_thread_stack, sizeof(dht11_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, dht11_read, NULL, "dht11_thread");

    // Main thread can be used to handle other tasks or enter an idle state
    while (1) {
        thread_yield(); // Yield control to the dht11_thread or other threads
    }
    return 0;
}