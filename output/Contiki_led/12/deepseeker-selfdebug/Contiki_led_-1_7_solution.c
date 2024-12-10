
#include "contiki.h"
#include "dev/dht11-sensor.h" // Include the DHT11 sensor header file
#include "dev/leds.h" // Include the LEDs driver header file
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED    (LEDS_RED)
#define RGB_LED_GREEN  (LEDS_GREEN)
#define RGB_LED_BLUE   (LEDS_BLUE)

// Define the period for reading the DHT11 sensor
#define READ_PERIOD   (5 * CLOCK_SECOND)

// Function to read the DHT11 sensor and turn on the RGB LED
static void read_dht11_and_toggle_led(void) {
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    int status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer);
    if (status == DHT11_STATUS_OKAY) {
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_decimal);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_decimal);

        printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);

        // Turn on the RGB LED
        leds_set(RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE);
    } else {
        printf("Failed to read DHT11 sensor, status: %d\n", status);
    }
}

// Process to handle the periodic reading of the DHT11 sensor and toggling the LED
PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PIN, 1);

    // Set the timer for the periodic reading
    etimer_set(&timer, READ_PERIOD);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor and toggle the LED
        read_dht11_and_toggle_led();

        // Reset the timer for the next reading
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Start the Contiki system
    contiki_init();

    // Start the DHT11 and LED process
    process_start(&dht11_led_process, NULL);

    return 0;
}
