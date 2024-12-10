#include <stdio.h>
#include "sys/ctimer.h"
#include "sensors.h"
#include "contiki-default-conf.h"

// Assuming the RGB LED is connected and configured accordingly.
#define RGB_LED_RED 1
#define RGB_LED_GREEN 2
#define RGB_LED_BLUE 4

static struct ctimer read_timer;

void read_dht11_callback(void *ptr) {
    int humidity, temperature;

    // Simulate reading humidity and temperature from DHT11 sensor
    // Replace these with actual sensor read functions
    humidity = 55; // (%)
    temperature = 23; // (°C)

    printf("Humidity: %d%%, Temperature: %d°C\n", humidity, temperature);

    // Turn on RGB LED (e.g., white if both red and green are on)
    int led_status = RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE;
    printf("Turning on RGB LED with status: %d\n", led_status);
    
    // TODO: Code to turn on the RGB LED with specified color
    // Example: set_rgb_led(led_status);

    // Reset the timer to trigger after 5 seconds
    ctimer_reset(&read_timer);
}

int main(void) {
    // Initialize the clock system
    clock_init();
    
    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    // Start a periodic timer to read DHT11 every 5 seconds
    ctimer_set(&read_timer, CLOCK_SECOND * 5, read_dht11_callback, NULL);

    // Main loop, can do other tasks here
    while (1) {
        // Yield the processor to allow low power mode
        // and other processes to run
        PROCESS_WAIT_EVENT();
    }

    return 0;
}