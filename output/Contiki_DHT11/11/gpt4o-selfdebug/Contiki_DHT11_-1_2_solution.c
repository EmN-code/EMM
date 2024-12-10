#include "contiki.h"
#include "dev/dht11/dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data)
{
  PROCESS_BEGIN();

  SENSORS_ACTIVATE(dht11_sensor);

  while (1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the sensor values
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    // Check for sensing errors
    if (temperature != -1 && humidity != -1) {
      printf("Temperature: %d°C, Humidity: %d%\n", temperature, humidity);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  SENSORS_DEACTIVATE(dht11_sensor);

  PROCESS_END();
}