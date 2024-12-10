
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
    leds_on(LEDS_RED);
    leds_on(LEDS_GREEN);
    leds_on(LEDS_BLUE);
}

// Function to read DHT11 sensor
void read_dht11() {
    int16_t temperature, humidity;
    if (dht11_read(&temperature, &humidity) == 0) {
        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_PIN);

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        read_dht11();

        // Turn on the RGB LED
        turn_on_rgb_led();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    // Initialize the Contiki system
    contiki_init();

    // Start the Contiki processes
    contiki_process_run();

    return 0;
}
