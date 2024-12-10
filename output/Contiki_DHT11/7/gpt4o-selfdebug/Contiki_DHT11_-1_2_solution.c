#include "sensors.h"
#include "contiki.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data)
{
    PROCESS_BEGIN();

    printf("DHT11 process started.\n");

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        SENSORS_ACTIVATE(dht11_sensor);

        int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        if (temperature != -1 && humidity != -1) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                    temperature / 10, temperature % 10,
                    humidity / 10, humidity % 10);
        } else {
            printf("Failed to read the DHT11 sensor!\n");
        }

        SENSORS_DEACTIVATE(dht11_sensor);
        etimer_reset(&timer);
    }

    PROCESS_END();
}