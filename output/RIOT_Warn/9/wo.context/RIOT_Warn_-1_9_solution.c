#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"
#include <stdio.h>

#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds
#define TEMP_THRESHOLD 30.0 // Temperature threshold
#define DHT_PIN GPIO_PIN(PORT_A, 0) // Assuming PORT_A is correct and physical pin 0
#define LED_PIN GPIO_PIN(PORT_A, 1) // Assuming LED is connected to PORT_A, pin 1

void *temperature_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            float temp_celcius = (float)temp / 10.0;
            printf("Temperature: %.1f°C\n", temp_celcius);

            if (temp_celcius > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeded threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create temperature thread
    kernel_pid_t pid = thread_create(stack, sizeof(stack), 
                                     THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                     temperature_thread, NULL, "temperature_thread");
    (void)pid; // Avoid unused variable warning

    return 0;
}