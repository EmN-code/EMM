#include "contiki.h"
#include "sys/etimer.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer et;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    if(temperature != -1 && humidity != -1) {
      printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
      printf("Failed to read sensor\n");
    }
  }

  SENSORS_DEACTIVATE(dht11_sensor);

  PROCESS_END();
}