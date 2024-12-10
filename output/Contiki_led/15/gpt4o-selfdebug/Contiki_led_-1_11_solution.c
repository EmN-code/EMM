#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Reading and LED Control");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    static int humidity_integer, humidity_decimal;
    static int temperature_integer, temperature_decimal;
    
    PROCESS_BEGIN();
    
    // Initialize the sensor and LEDs
    SENSORS_ACTIVATE(dht11_sensor);
    leds_off(LEDS_ALL);

    while(1) {
        // Set the timer to read every 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);
        
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read values from the DHT11 sensor
        humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
        temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        
        // Print the values
        printf("Humidity: %d.%d%%\n", humidity_integer, humidity_decimal);
        printf("Temperature: %d.%dC\n", temperature_integer, temperature_decimal);

        // Turn on RGB LED
        leds_on(LEDS_RED);
        etimer_set(&timer, CLOCK_SECOND * 1);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        leds_off(LEDS_RED);
    }

    PROCESS_END();
}