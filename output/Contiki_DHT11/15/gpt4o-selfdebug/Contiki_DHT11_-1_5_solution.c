#include "contiki.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Sensor activation
    SENSORS_ACTIVATE(dht11_sensor);

    // Set timer to read sensor every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        if(humidity != -1 && temperature != -1) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        }
        else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}