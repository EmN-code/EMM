#include "dev/sensor/dht11-sensor.h"
#include "contiki.h"
#include <stdio.h>

#define DHT11_PIN 2

PROCESS(dht11_sensor_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_sensor_process);

PROCESS_THREAD(dht11_sensor_process, ev, data)
{
  static struct etimer timer;
  int16_t temperature, humidity;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if(dht11_sensor.value(SENSOR_TYPE_TEMPERATURE) != -1) {
      temperature = dht11_sensor.value(SENSOR_TYPE_TEMPERATURE);
      humidity = dht11_sensor.value(SENSOR_TYPE_HUMIDITY);
      printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
        temperature / 10, temperature % 10,
        humidity / 10, humidity % 10);
    } else {
      printf("Failed to read the sensor\n");
    }
  }

  SENSORS_DEACTIVATE(dht11_sensor);
  PROCESS_END();
}