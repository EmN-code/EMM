#include "sensors/sensor.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();
  
  // Use the dht11 sensor resource
  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    // Set a timer of 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the event timer to timeout
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read data from the dht11 sensor
    int16_t temp = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int16_t humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    // Check for error
    if(temp != -1 && humidity != -1) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temp, humidity);
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}