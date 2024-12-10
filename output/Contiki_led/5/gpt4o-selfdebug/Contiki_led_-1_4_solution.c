#include <dev/leds.h>  // Include necessary hardware libraries for LEDs
#include <contiki-net.h> // Network stack for Contiki
#include <contiki-lib.h> // Core library for Contiki
#include <contiki-default-conf.h> // Default configuration
#include <contiki-conf.h> // General configuration

#include <stdio.h>
#include <stdlib.h>

#define DHT11_PIN 1

static struct etimer timer;

// Function to simulate reading from a DHT11 sensor
int read_dht11(int *temperature, int *humidity) {
    // Simulate successful reading
    *temperature = rand() % 30 + 20;  // Random values between 20 and 49
    *humidity = rand() % 100;         // Random values between 0 and 99
    return 0;  // Assume success
}

void set_rgb_led(int temperature, int humidity) {
    // Simple logic to turn on the RGB LED based on temperature and humidity
    if (temperature > 25 || humidity > 50) {
        leds_on(RGB_LED_RED);
    } else {
        leds_off(RGB_LED_RED);
    }
}

PROCESS(sensor_process, "DHT11 Sensor and LED Control");
AUTOSTART_PROCESSES(&sensor_process);

PROCESS_THREAD(sensor_process, ev, data) {
    static int temperature, humidity;

    PROCESS_BEGIN();

    while (1) {
        // Set timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the sensor
        read_dht11(&temperature, &humidity);
        printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);

        // Control the LED based on the read temperature and humidity
        set_rgb_led(temperature, humidity);
    }

    PROCESS_END();
}

int main() {
    // Contiki main function usually does nothing and never exits
    return 0;
}