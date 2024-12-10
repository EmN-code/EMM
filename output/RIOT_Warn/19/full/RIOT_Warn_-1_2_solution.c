#include <stdio.h>  // For standard input and output operations
#include <xtimer.h>  // For setting timers
#include <periph/gpio.h>  // For GPIO operations
#include "net/emcute.h"  // For MQTT-SN communication

#define THRESHOLD_TEMP 30
#define LOOP_DELAY (5U * US_PER_SEC)
#define GPIO_LED_PIN GPIO_PIN(PORT_A, 2)  // ESP32-specific GPIO pin for an LED
#define GPIO_DHT11_PIN GPIO_PIN(PORT_A, 3) // ESP32-specific GPIO pin for DHT11

// Function to simulate reading temperature from the DHT11 sensor
int read_temperature() {
    // Simulated value for testing purposes
    return 32;
}

int main(void) {
    gpio_init(GPIO_LED_PIN, GPIO_OUT);
    gpio_clear(GPIO_LED_PIN);

    // Reconnect logic in case of failed connection
    emcute_topic_t topic;
    while (1) {
        int rc = emcute_con(NULL, true, NULL, NULL, 0, 0);
        if (rc != EMCUTE_OK) {
            printf("Failed to connect, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected successfully\n");
            break;
        }
    }

    while (1) {
        int temperature = read_temperature();
        printf("Current temperature: %d\n", temperature);

        if (temperature > THRESHOLD_TEMP) {
            gpio_set(GPIO_LED_PIN);
            printf("Warning: Temperature exceeds threshold!\n");
        } else {
            gpio_clear(GPIO_LED_PIN);
        }

        xtimer_sleep(LOOP_DELAY);
    }
}