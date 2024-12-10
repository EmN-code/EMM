#include "contiki.h"
#include "dev/dht11.h"

PROCESS(read_dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the sensor
        int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        // Check for errors in sensor values
        if(temperature != DHT11_ERROR && humidity != DHT11_ERROR) {
            printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}