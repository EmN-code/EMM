
#include "contiki.h"
#include "dev/dht11-sensor.h"  // Ensure this header file is available
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int humidity, temperature;

    PROCESS_BEGIN();

    // Configure the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor values
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Print the values
        printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

        // Turn on the RGB LED
        leds_on(LEDS_RED);

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
