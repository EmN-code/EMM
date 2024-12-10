#include "contiki.h"
#include "dev/gpio.h"
#include "dev/dht11/dht11-sensor.h"

PROCESS(dht11_reading_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_reading_process);

PROCESS_THREAD(dht11_reading_process, ev, data)
{
    static struct etimer timer;
    int temperature, humidity;

    PROCESS_BEGIN();

    // Initialize the sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
        // Set timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity
        if (dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER) != -1 &&
            dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER) != -1) {
            temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
        } else {
            printf("Error reading from DHT11 sensor\n");
        }
    }

    // Deactivate the sensor
    SENSORS_DEACTIVATE(dht11_sensor);

    PROCESS_END();
}