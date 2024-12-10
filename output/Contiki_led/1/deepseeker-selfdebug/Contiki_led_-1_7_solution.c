
#include "contiki.h"
#include "dev/dht11-sensor.h"  // Ensure this header is included
#include "dev/leds.h"
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED    (LEDS_RED)
#define RGB_LED_GREEN  (LEDS_GREEN)
#define RGB_LED_BLUE   (LEDS_BLUE)

// Define the RGB LED color to turn on
#define RGB_LED_COLOR  (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

// Process to read DHT11 sensor and turn on RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
    static struct etimer timer;
    static int temperature;
    static int humidity;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        // Print the temperature and humidity
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

        // Turn on the RGB LED
        leds_set(RGB_LED_COLOR);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    // Start the Contiki system
    contiki_init();

    // Start the DHT11 and RGB LED process
    process_start(&dht11_rgb_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
