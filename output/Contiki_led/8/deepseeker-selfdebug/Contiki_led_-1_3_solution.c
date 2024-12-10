
#include "contiki.h"
#include "dev/dht11.h"  // Ensure this header file is available
#include "dev/leds.h"   // For controlling the RGB LED
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

// Function to read DHT11 sensor data
static void read_dht11(void) {
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    int status = dht11_read(&humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal);

    if (status == DHT11_STATUS_OKAY) {
        printf("DHT11 - Humidity: %d.%d%%, Temperature: %d.%d°C\n",
               humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
    } else {
        printf("DHT11 read failed with status: %d\n", status);
    }
}

// Process to handle the periodic reading of the DHT11 sensor and controlling the RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        read_dht11();

        // Turn on the RGB LED (e.g., set it to white)
        leds_set(RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE);

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

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
