#include <ztimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mqtt.h>
#include <evmsg.h>
#include <timex.h>
#include <kernel.h>
#include <dht.h>
#include <gpio.h>
#include <paho_mqtt.h>

// Constants
#define TEMP_THRESHOLD 30  // Temperature threshold in celsius

// DHT Parameters
dht_t dev;
gpio_t dht_gpio = GPIO_PIN(0, 4);  // Assuming the DHT sensor is connected to GPIO4

// LED Parameters
gpio_t led_gpio = GPIO_PIN(0, 2);  // Assuming the LED is connected to GPIO2

void read_dht_and_control_led(void) {
    int16_t temp, hum;
    while (1) {
        // Read temperature and humidity from DHT sensor
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temp: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // If temperature exceeds threshold, turn on LED
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(led_gpio);
            } else {
                gpio_clear(led_gpio);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize the DHT sensor
    dev.pin = dht_gpio;
    dev.type = DHT11;
    
    // Initialize LED GPIO
    gpio_init(led_gpio, GPIO_OUT);

    // Create a thread for reading the DHT sensor and controlling the LED
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_dht_and_control_led, NULL, "dht_reader");
    
    return 0;
}