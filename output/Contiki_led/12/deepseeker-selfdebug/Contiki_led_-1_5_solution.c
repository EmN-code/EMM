
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "contiki.h"
#include "dev/dht11-sensor.h" // Ensure this header is available
#include "dev/leds.h" // Assuming you have a LED driver

// Define the RGB LED pins
#define RED_LED_PIN   5
#define GREEN_LED_PIN 6
#define BLUE_LED_PIN  7

// Function to read DHT11 sensor data
void read_dht11(void) {
    uint8_t humidity_integer, humidity_decimal;
    uint8_t temperature_integer, temperature_decimal;
    uint8_t status;

    // Read the sensor data
    status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer);
    if (status != DHT11_STATUS_OKAY) {
        printf("Failed to read humidity integer part\n");
        return;
    }

    status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_decimal);
    if (status != DHT11_STATUS_OKAY) {
        printf("Failed to read humidity decimal part\n");
        return;
    }

    status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer);
    if (status != DHT11_STATUS_OKAY) {
        printf("Failed to read temperature integer part\n");
        return;
    }

    status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_decimal);
    if (status != DHT11_STATUS_OKAY) {
        printf("Failed to read temperature decimal part\n");
        return;
    }

    // Print the sensor data
    printf("Humidity: %d.%d %% Temperature: %d.%d C\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);

    // Turn on the RGB LED based on the temperature
    if (temperature_integer < 20) {
        leds_single_on(RED_LED_PIN);
        leds_single_off(GREEN_LED_PIN);
        leds_single_off(BLUE_LED_PIN);
    } else if (temperature_integer >= 20 && temperature_integer < 30) {
        leds_single_off(RED_LED_PIN);
        leds_single_on(GREEN_LED_PIN);
        leds_single_off(BLUE_LED_PIN);
    } else {
        leds_single_off(RED_LED_PIN);
        leds_single_off(GREEN_LED_PIN);
        leds_single_on(BLUE_LED_PIN);
    }
}

// Process to handle the periodic reading of the DHT11 sensor
PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PIN, 2);

    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        read_dht11();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    contiki_init();

    // Start the DHT11 reader process
    process_start(&dht11_reader_process, NULL);

    return 0;
}
