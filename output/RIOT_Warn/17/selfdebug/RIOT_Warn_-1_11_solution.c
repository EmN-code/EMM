#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(0, 18)  // Use the actual pin connected to DHT11
#define LED_PIN GPIO_PIN(0, 2)     // Use the actual pin connected to the LED
#define TEMP_THRESHOLD 30          // Temperature threshold

// Mock function to simulate reading from a DHT11 sensor
int read_dht11_temperature(void) {
    // In actual code, read from the sensor and return temperature
    return rand() % 40;  // Random temperature between 0 and 39
}

void *temperature_monitor(void *arg) {
    (void)arg;
    while (1) {
        int temperature = read_dht11_temperature();
        printf("Current temperature: %d°C\n", temperature);

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Temperature exceeds threshold! LED turned ON.\n");
        } else {
            gpio_clear(LED_PIN);
            printf("Temperature is below threshold. LED turned OFF.\n");
        }

        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    puts("Temperature monitor started...");

    temperature_monitor(NULL);

    return 0;
}