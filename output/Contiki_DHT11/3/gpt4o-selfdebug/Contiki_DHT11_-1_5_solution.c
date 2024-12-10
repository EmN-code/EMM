#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki.h"

#include <stdio.h>

static struct etimer timer;

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  PROCESS_BEGIN();
  
  // Configure the DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the temperature and humidity from the DHT11 sensor
    int temp_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humi_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    if (temp_integer != -1 && humi_integer != -1) {
      printf("Temperature: %d°C, Humidity: %d%%\n", temp_integer, humi_integer);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}