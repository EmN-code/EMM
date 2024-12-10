#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sensors.h"  // Include the sensors header for DHT11
#include "dev/dht11-sensor.h" // Ensure this path matches your setup.
#include "contiki.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();
    
    SENSORS_ACTIVATE(dht11_sensor);

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        if(temperature >= 0 && humidity >= 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        }
        else {
            printf("Failed to read from DHT11 sensor.\n");
        }
    }

    SENSORS_DEACTIVATE(dht11_sensor);

    PROCESS_END();
}
