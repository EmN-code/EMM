
#include "contiki.h"
#include "dev/leds.h"
#include "dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    static int temp_integer, temp_decimal, hum_integer, hum_decimal;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read DHT11 sensor values
        temp_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        temp_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        hum_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        hum_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

        // Turn on the RGB LED
        leds_set(LEDS_RED);

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    // Initialize the Contiki system
    contiki_init();

    // Start the process
    process_start(&dht11_led_process, NULL);

    // Enter the main loop
    while(1) {
        process_run();
    }

    return 0;
}
