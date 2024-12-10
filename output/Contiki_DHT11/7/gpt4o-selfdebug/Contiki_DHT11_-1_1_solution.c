#include "sensors.h" // Including in descending alphabetical order
#include "process.h"
#include "etimer.h"
#include "contiki.h"

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Initialize the DHT11 Sensor
    SENSORS_ACTIVATE(dht11_sensor);

    // Set an event timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the periodic timer event to be triggered
        PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Read temperature and humidity values
        int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
        int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

        // Print the sensor values
        printf("Temperature: %d C\n", temperature);
        printf("Humidity: %d %%\n", humidity);

        // Reset the timer for the next read
        etimer_reset(&timer);
    }

    SENSORS_DEACTIVATE(dht11_sensor);
    PROCESS_END();
}