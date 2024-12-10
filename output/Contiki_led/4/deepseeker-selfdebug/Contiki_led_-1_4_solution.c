
#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h" // Ensure this header file is available

// Define the RGB LED colors
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)

// Function to set the RGB LED color
void rgb_led_set(uint8_t colour) {
    leds_set(colour);
}

// Process to read DHT11 sensor and control RGB LED
PROCESS(dht11_rgb_process, "DHT11 RGB Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;
    static uint8_t humidity_integer, temperature_integer;
    static uint8_t humidity_decimal, temperature_decimal;
    static uint8_t status;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0); // Replace with actual GPIO port
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 0);  // Replace with actual GPIO pin

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        status = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer);
        if (status == DHT11_STATUS_OKAY) {
            dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer);
            printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);

            // Turn on the RGB LED
            rgb_led_set(RGB_LED_WHITE);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_rgb_process, NULL);

    // Enter the main loop
    while(1) {
        process_run();
    }

    return 0;
}
