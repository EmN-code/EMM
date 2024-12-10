#include "sensors.h"
#include "dev/gpio-hal.h"
#include "contiki.h"

#include <stdio.h>

// Declaration of the DHT11 sensor
const struct sensors_sensor dht11_sensor;

PROCESS(dht11_read_process, "DHT11 Sensor Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    int temp_int;
    int humidity_int;

    PROCESS_BEGIN();

    // Configure GPIO
    sensors_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    sensors_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        sensors_sensor.configure(SENSORS_ACTIVE, 1);

        temp_int = sensors_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity_int = sensors_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        printf("Temperature: %d°C\n", temp_int);
        printf("Humidity: %d%%\n", humidity_int);
    }

    PROCESS_END();
}