#include <contiki-default-conf.h>
#include <stdio.h>
#include <stdlib.h>
#include "dev/dht11-sensor.h"
#include "dev/leds.h"
#include "dev/sensor-sensor.h"
#include "sys/etimer.h"

PROCESS(dht11_rgbled_process, "DHT11 and RGB LED Control Process");
AUTOSTART_PROCESSES(&dht11_rgbled_process);

PROCESS_THREAD(dht11_rgbled_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity from DHT11 sensor
        temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        if(temperature != -1 && humidity != -1) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Turn on the RGB LED to indicate successful reading
            leds_on(LEDS_RED);

            // Wait for a short duration to keep LED on
            etimer_set(&timer, CLOCK_SECOND * 1);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

            // Turn off the RGB LED
            leds_off(LEDS_RED);
        }
        else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}
