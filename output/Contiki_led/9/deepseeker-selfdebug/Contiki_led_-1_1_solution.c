
#include "contiki.h"
#include "dev/dht11.h"
#include "dev/leds.h"
#include <stdio.h>

// Define the RGB LED pins
#define RED_LED_PIN 22
#define GREEN_LED_PIN 23
#define BLUE_LED_PIN 24

// Function to turn on the RGB LED
void turn_on_rgb_led() {
    leds_single_on(RED_LED_PIN);
    leds_single_on(GREEN_LED_PIN);
    leds_single_on(BLUE_LED_PIN);
}

// Function to read DHT11 sensor
void read_dht11() {
    int16_t temperature, humidity;
    if (dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

// Process for reading DHT11 and turning on RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_init();

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor
        read_dht11();

        // Turn on the RGB LED
        turn_on_rgb_led();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    // Start the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_rgb_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}