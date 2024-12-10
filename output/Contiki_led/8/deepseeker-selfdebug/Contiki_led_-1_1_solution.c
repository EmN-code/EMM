
#include "contiki.h"
#include "dev/dht11.h"
#include "dev/leds.h"
#include <stdio.h>

// Define the RGB LED pins
#define RED_LED_PIN 22
#define GREEN_LED_PIN 23
#define BLUE_LED_PIN 24

// Define the DHT11 sensor
extern const struct sensors_sensor dht11_sensor;

// Function to read DHT11 sensor data
static void read_dht11(void) {
    int temp, hum;
    if (dht11_sensor.value(&dht11_sensor, 0, &temp, &hum) == 0) {
        printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

// Function to turn on the RGB LED
static void turn_on_rgb_led(void) {
    leds_single_on(RED_LED_PIN);
    leds_single_on(GREEN_LED_PIN);
    leds_single_on(BLUE_LED_PIN);
}

// Process for reading DHT11 and turning on the RGB LED
PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(&dht11_sensor, SENSORS_ACTIVE, 1);

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor data
        read_dht11();

        // Turn on the RGB LED
        turn_on_rgb_led();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_rgb_led_process, NULL);

    return 0;
}
