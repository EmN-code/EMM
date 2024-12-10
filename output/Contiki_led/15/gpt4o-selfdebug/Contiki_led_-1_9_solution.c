#include "sensors.h"
#include "dht11_sensor.h"
#include <stdio.h>
#include <contiki.h>

PROCESS(read_dht11_process, "DHT11 Reader");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
        
        int16_t humidity_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int16_t temperature_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        if(humidity_int != -1 && temperature_int != -1) {
            // Assuming that we turn on the RGB LED based on the sensor reading,
            // a real function should replace the printf with GPIO operations.
            printf("Humidity: %d%%, Temperature: %d°C\n", humidity_int, temperature_int);
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}