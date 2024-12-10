#include "sensors.h"
#include "dht11_sensor.h"
#include "contiki.h"
#include <stdio.h>

static struct etimer timer;

PROCESS(dht11_process, "DHT11 Sensor and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    PROCESS_BEGIN();

    // Configure the GPIO pins for the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait until the timer expires
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read temperature and humidity values from the DHT11 sensor
        int dec_temp = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        int int_temp = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int dec_hum = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
        int int_hum = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        if (int_temp != -1 && int_hum != -1) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", int_temp, dec_temp, int_hum, dec_hum);
            // Turn on RGB LED here
        } else {
            printf("Failed to read from DHT11 sensor.\n");
        }
    }

    SENSORS_DEACTIVATE(dht11_sensor);
    PROCESS_END();
}