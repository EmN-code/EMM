#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    int humidity_int;
    int temperature_int;

    PROCESS_BEGIN();

    // Configure the GPIO
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the values from the sensor
        humidity_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Check if values are valid
        if (humidity_int != -1 && temperature_int != -1) {
            printf("Temperature: %d C \t Humidity: %d %%\n", temperature_int, humidity_int);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}
