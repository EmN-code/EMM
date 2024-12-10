
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include <stdio.h>

// Define the GPIO pins for the DHT11 sensor
#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

// Define the RGB LED colors
#define RGB_LED_RED     (1 << 0)
#define RGB_LED_GREEN   (1 << 1)
#define RGB_LED_BLUE    (1 << 2)

// Function to read DHT11 sensor data
static int read_dht11(int *temperature, int *humidity) {
    // Placeholder function for reading DHT11 sensor data
    // This function should be implemented according to the DHT11 sensor's datasheet
    *temperature = 25; // Example temperature value
    *humidity = 60;    // Example humidity value
    return 0; // Return 0 on success
}

// Function to control the RGB LED
static void control_rgb_led(int temperature, int humidity) {
    if (temperature > 30) {
        leds_set(RGB_LED_RED); // Turn on red LED if temperature is high
    } else if (humidity > 70) {
        leds_set(RGB_LED_BLUE); // Turn on blue LED if humidity is high
    } else {
        leds_set(RGB_LED_GREEN); // Turn on green LED otherwise
    }
}

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Initialize GPIO for DHT11 sensor
    gpio_set_mode(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_MODE_INPUT);

    // Set up the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor data
        if (read_dht11(&temperature, &humidity) == 0) {
            printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
            // Control the RGB LED based on sensor data
            control_rgb_led(temperature, humidity);
        } else {
            printf("Failed to read DHT11 sensor data\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
