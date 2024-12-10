#include "sensors.h"
#include <stdio.h>
#include "contiki.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data)
{
    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int temperature_integer = DHT11_VALUE_TEMPERATURE_INTEGER; 
        int humidity_integer = DHT11_VALUE_HUMIDITY_INTEGER;

        printf("Temperature: %d\n", temperature_integer);
        printf("Humidity: %d\n", humidity_integer);

        etimer_reset(&timer);
    }

    PROCESS_END();
}
