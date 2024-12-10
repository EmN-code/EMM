
#include "contiki.h"
#include "dev/dht11.h"
#include "dev/leds.h"
#include <stdio.h>

// Define the RGB LED pins
#define RED_LED_PIN 22
#define GREEN_LED_PIN 23
#define BLUE_LED_PIN 24

// Function to initialize the DHT11 sensor
void dht11_init(void) {
    dht11_sensor.configure(DHT11_PIN);
}

// Function to read temperature from DHT11
int dht11_read_temperature(void) {
    int temperature;
    dht11_sensor.value(DHT11_VALUE_TEMP, &temperature);
    return temperature;
}

// Function to turn on the RGB LED based on temperature
void set_rgb_led(int temperature) {
    if (temperature < 20) {
        // Blue color for low temperature
        leds_single_on(BLUE_LED_PIN);
        leds_single_off(RED_LED_PIN);
        leds_single_off(GREEN_LED_PIN);
    } else if (temperature >= 20 && temperature < 30) {
        // Green color for moderate temperature
        leds_single_off(BLUE_LED_PIN);
        leds_single_on(GREEN_LED_PIN);
        leds_single_off(RED_LED_PIN);
    } else {
        // Red color for high temperature
        leds_single_off(BLUE_LED_PIN);
        leds_single_off(GREEN_LED_PIN);
        leds_single_on(RED_LED_PIN);
    }
}

// Process for reading DHT11 and controlling RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
    static struct etimer timer;
    static int temperature;

    PROCESS_BEGIN();

    // Initialize DHT11 sensor
    dht11_init();

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature from DHT11
        temperature = dht11_read_temperature();

        // Turn on the RGB LED based on temperature
        set_rgb_led(temperature);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Start the Contiki system
    contiki_init();

    // Start the DHT11 and RGB LED process
    process_start(&dht11_rgb_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
