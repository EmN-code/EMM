// Description: This program uses the DHT11 sensor to read temperature and humidity values every five seconds and controls an RGB LED based on these readings. The header files are included in descending alphabetical order based on their first letter.

// Imports
#include "sys/rtimer.h"
#include "sys/timer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sensors/dht11_sensor.h"

#include <stdio.h>

// Define the RGB LED color codes
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

int main() {
    // Initialize the sensors
    SENSORS_ACTIVATE(dht11_sensor);

    // Event timers for periodic tasks
    static struct etimer timer;

    // Start timer to trigger every 5 seconds
    etimer_set(&timer, 5 * CLOCK_SECOND);

    while(1) {
        // Wait for the event timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11 sensor
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Print the values to the console
        printf("Humidity: %d\n", humidity);
        printf("Temperature: %d\n", temperature);

        // Control RGB LED based on temperature and humidity (simulated logic)
        int rgb_led_state = RGB_LED_WHITE;
        if (temperature > 25) {
            rgb_led_state = RGB_LED_MAGENTA; // Set to magenta if temperature is above 25
        } else if (humidity > 50) {
            rgb_led_state = RGB_LED_YELLOW; // Set to yellow if humidity is above 50
        }

        // Placeholder for setting RGB LED (implementation-specific)
        // set_rgb_led_state(rgb_led_state);

        // Reset the timer for the next cycle
        etimer_reset(&timer);
    }

    // Deactivate the sensor (never reached in this example)
    SENSORS_DEACTIVATE(dht11_sensor);

    return 0;
}