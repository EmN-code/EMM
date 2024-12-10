#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    while (1) {
        etimer_set(&timer, 5 * CLOCK_SECOND);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        SENSORS_ACTIVATE(dht11_sensor);

        int temp_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int hum_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        printf("Temperature: %d\n", temp_integer);
        printf("Humidity: %d\n", hum_integer);

        // Activate the RGB LED - Example placeholder
        // RGB_LED_ON(RGB_LED_GREEN);

        SENSORS_DEACTIVATE(dht11_sensor);
    }

    PROCESS_END();
}