#include "contiki.h"
#include "sensors.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data)
{
  PROCESS_BEGIN();
  
  printf("Starting DHT11 sensor process\n");
  
  // Set the timer to trigger every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
    
    // Read from the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);
    SENSORS_DEACTIVATE(dht11_sensor);

    if (temperature != -1 && humidity != -1) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
    
    // Reset the timer
    etimer_reset(&timer);
  }
  
  PROCESS_END();
}