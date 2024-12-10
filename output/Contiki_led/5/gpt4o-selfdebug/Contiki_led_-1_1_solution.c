#include "sensors_sensor.h"
#include "contiki-default-conf.h"

#include <stdio.h>
#include <stddef.h>

extern const struct sensors_sensor dht11_sensor;

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

static struct etimer timer;

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    PROCESS_BEGIN();

    etimer_set(&timer, 5 * CLOCK_SECOND);
    
    printf("Reading DHT11 values every 5 seconds\n");

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int temperature, humidity;
        if (dht11_sensor.value(SENSORS_READY)) {
            temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
            humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

            // Simulate turning on an RGB LED as a placeholder
            printf("RGB LED ON\n");
        } else {
            printf("DHT11 Sensor is not ready\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}
