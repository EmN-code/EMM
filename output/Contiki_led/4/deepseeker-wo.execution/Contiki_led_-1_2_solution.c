
#include "contiki-net.h"
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "contiki-default-conf.h"
#include "tcp-socket.h"
#include "dev/leds.h"
#include "dev/rgb-led/rgb-led.h"
#include "dev/radio.h"
#include "contiki-conf.h"
#include "dev/button-hal.h"
#include "contiki-lib.h"

#include <stdio.h>
#include <stdint.h>

// Define the GPIO pin for the DHT11 sensor
#define DHT11_GPIO_PIN 2

// Function to read DHT11 sensor data
static int read_dht11(uint8_t *humidity, uint8_t *temperature) {
    // Simulate reading DHT11 sensor data
    // In a real implementation, you would use GPIO functions to read the sensor
    *humidity = 50; // Example humidity value
    *temperature = 25; // Example temperature value
    return 0; // Success
}

// Process to read DHT11 sensor and turn on RGB LED
PROCESS(dht11_rgb_led_process, "DHT11 RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;
    static uint8_t humidity, temperature;

    PROCESS_BEGIN();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor data
        if (read_dht11(&humidity, &temperature) == 0) {
            printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

            // Turn on the RGB LED
            leds_set(LEDS_RED);
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

    // Start the DHT11 RGB LED process
    process_start(&dht11_rgb_led_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
