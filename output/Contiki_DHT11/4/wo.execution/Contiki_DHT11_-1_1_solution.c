#include <stdio.h>
#include "contiki.h"
#include "dev/dht11.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  // Initialize DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  // Set up a timer that expires every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read sensor values
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    // Print the values
    printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}
