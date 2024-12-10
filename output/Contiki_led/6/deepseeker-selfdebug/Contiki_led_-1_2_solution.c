
#include "contiki.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED    0x01
#define RGB_LED_GREEN  0x02
#define RGB_LED_BLUE   0x04
#define RGB_LED_CYAN   (RGB_LED_GREEN | RGB_LED_BLUE)

// Define the DHT11 sensor pin (assuming it's connected to a specific GPIO pin)
#define DHT11_PIN 2

// Function to simulate reading DHT11 (since dht11.h is not available)
void read_dht11() {
    // Simulate reading DHT11 data
    printf("DHT11 data read (simulated)\n");
}

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor data
        read_dht11();

        // Turn on the RGB LED (Cyan color)
        leds_set(RGB_LED_CYAN);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
