
#include "contiki.h"
#include "dev/dht11-sensor.h" // Ensure this header is included
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int humidity, temperature;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0); // Replace with actual GPIO port
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 0);  // Replace with actual GPIO pin

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Print the readings
        printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

        // Turn on the RGB LED
        leds_set(LEDS_RED); // Assuming LEDS_RED is defined in leds.h

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    // Start the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_led_process, NULL);

    // Enter the Contiki main loop
    while(1) {
        process_run();
    }

    return 0;
}
