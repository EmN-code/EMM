#include "sensors.h"
#include "sys/timer.h"
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "contiki.h"
#include <stdio.h>
#include "dht11-sensor.h"

PROCESS(dht11_process, "DHT11 sensor process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    static int temperature_integer;
    static int humidity_integer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while (1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature
        temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Read humidity
        humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        // Print the temperature and humidity
        printf("Temperature: %d C, Humidity: %d %%\n", temperature_integer, humidity_integer);
    }

    PROCESS_END();
}