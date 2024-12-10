#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>

#define GPIO_PIN_LED 2  // Assume GPIO2 for LED
#define TEMP_THRESHOLD 25.0  // Temperature threshold for LED warning

// Function to simulate reading temperature from DHT11 sensor
float read_temperature() {
    // In a real implementation, this function would interface with hardware
    // Here we just return a simulated temperature
    return 26.5;  // Simulate a temperature
}

void control_led(float temperature) {
    if (temperature > TEMP_THRESHOLD) {
        printf("Temperature %.2f exceeds threshold. LED is ON.\n", temperature);
        // Code to turn on LED
    } else {
        printf("Temperature %.2f is below threshold. LED is OFF.\n", temperature);
        // Code to turn off LED
    }
}

int main(void) {
    while (1) {
        float temperature = read_temperature();
        control_led(temperature);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}