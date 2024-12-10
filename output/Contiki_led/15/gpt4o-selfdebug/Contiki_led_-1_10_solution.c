#include "sensors.h"
#include <stdio.h>
#include "contiki.h"
#include "dht11_sensor.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;
    PROCESS_BEGIN();

    SENSORS_ACTIVATE(dht11_sensor);

    while (1) {
        // Set the timer to fire every 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity data
        int humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
        int temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);

        if (dht11_sensor.status == DHT11_STATUS_OKAY) {
            printf("Humidity: %d.%d%% Temperature: %d.%dC\n", 
                   humidity_integer, humidity_decimal, 
                   temperature_integer, temperature_decimal);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // This would be the point to activate the RGB LED
        // Example: rgb_led_on(color);
    }

    PROCESS_END();
}