#include "sensors.h"
#include "sys/timer.h"
#include <stdio.h>

const struct sensors_sensor dht11_sensor;

void read_dht11();
void rtimer_example_wait();

int main() {
    struct timer t;

    // Initialize the LED
    uint8_t led_status = 0;

    // Main loop
    while (1) {
        // Set timer for 5 seconds
        timer_set(&t, CLOCK_SECOND * 5);

        // Read DHT11 sensor
        read_dht11();

        // Toggle LED status
        led_status = !led_status;
        if (led_status) {
            printf("RGB LED is ON\n");
            // Add code to turn the RGB LED on here
        } else {
            printf("RGB LED is OFF\n");
            // Add code to turn the RGB LED off here
        }

        // Wait for the timer to expire
        while (!timer_expired(&t)) {
            // Do nothing
        }
    }

    return 0;
}

void read_dht11() {
    printf("Reading DHT11 sensor...\n");

    // Simulate reading sensor data
    int humidity_integer = DHT11_VALUE_HUMIDITY_INTEGER;
    int temperature_integer = DHT11_VALUE_TEMPERATURE_INTEGER;

    printf("Humidity: %d%%, Temperature: %dC\n", humidity_integer, temperature_integer);
}

void rtimer_example_wait() {
    printf("Wait function simulated.\n");
}