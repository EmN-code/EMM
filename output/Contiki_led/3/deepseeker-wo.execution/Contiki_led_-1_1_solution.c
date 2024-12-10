
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include <stdio.h>

// Define the RGB LED pins
#define RED_LED_PIN 22
#define GREEN_LED_PIN 23
#define BLUE_LED_PIN 24

// Function to initialize the DHT11 sensor
void dht11_init(void) {
    dht11_sensor.configure(DHT11_PIN, DHT11_MODE_SINGLE_READ);
}

// Function to read temperature and humidity from DHT11
void read_dht11(void) {
    int16_t temperature, humidity;
    if (dht11_sensor.value(DHT11_SENSOR_TYPE_TEMP, &temperature) == 0 &&
        dht11_sensor.value(DHT11_SENSOR_TYPE_HUMIDITY, &humidity) == 0) {
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }
}

// Function to turn on the RGB LED
void turn_on_rgb_led(void) {
    leds_on(LEDS_RED | LEDS_GREEN | LEDS_BLUE);
}

// Main function
int main(void) {
    // Initialize the DHT11 sensor
    dht11_init();

    // Initialize the RGB LED pins
    leds_init();

    // Set up a periodic timer to read the DHT11 sensor every 5 seconds
    static struct etimer timer;
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        read_dht11();

        // Turn on the RGB LED
        turn_on_rgb_led();

        // Reset the timer for the next reading
        etimer_reset(&timer);
    }

    return 0;
}
