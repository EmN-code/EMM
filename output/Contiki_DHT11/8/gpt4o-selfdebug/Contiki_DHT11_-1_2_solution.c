#include "contiki.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    /* Configure the DHT11 sensor */
    SENSORS_ACTIVATE(dht11_sensor);

    while (1) {
        /* Set the timer to trigger every 5 seconds */
        etimer_set(&timer, CLOCK_SECOND * 5);

        /* Wait for the timer to expire */
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        /* Read temperature and humidity from the DHT11 sensor */
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
    }

    SENSORS_DEACTIVATE(dht11_sensor);

    PROCESS_END();
}