#include "sys/timer.h"
#include "sys/rtimer.h"
#include "sensors.h"
#include "dev/dht11-sensor.h"

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data)
{
    PROCESS_BEGIN();

    while(1) {
        // Set the timer
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait until the timer expires
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        int temperature_integer = 0;
        int humidity_integer = 0;

        if(SENSORS_ACTIVATE(dht11_sensor) == DHT11_STATUS_OKAY) {
            temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            SENSORS_DEACTIVATE(dht11_sensor);
        }

        // Logic to turn on the RGB LED would go here, this
        // is hardware/platform-specific
        printf("Temperature: %d degrees, Humidity: %d percent\n", temperature_integer, humidity_integer);
    }
    
    PROCESS_END();
}