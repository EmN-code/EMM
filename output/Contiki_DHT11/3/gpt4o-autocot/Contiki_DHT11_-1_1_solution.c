#include "contiki.h"
#include "dev/leds.h"
#include "dev/sensor/dht11-sensor.h"
#include <stdio.h>

PROCESS(read_dht11_process, "DHT11 reading process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    /* Initialize the DHT11 sensor */
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
        /* Set timer to 5 seconds */
        etimer_set(&timer, CLOCK_SECOND * 5);

        /* Wait for the timer event */
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        /* Read values from the DHT11 sensor */
        int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        /* Check for reading errors */
        if (temperature != -1 && humidity != -1) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        } else {
            printf("Error reading DHT11 sensor\n");
        }
    }

    PROCESS_END();
}