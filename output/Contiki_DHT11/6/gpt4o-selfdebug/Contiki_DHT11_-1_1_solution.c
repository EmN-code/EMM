#include "contiki.h"
#include "dev/dht11/dht11-sensor.h"
#include <stdio.h>

#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Sensor Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    // Set timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read temperature and humidity from the DHT11 sensor
    temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    if (temperature != -1 && humidity != -1) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    } else {
      printf("Failed to read DHT11 sensor\n");
    }
  }

  PROCESS_END();
}