#include "dev/dht11/dht11-sensor.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(read_dht11_process, "DHT11 Sensor Read Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature;
  static int humidity;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    if(temperature != -1 && humidity != -1) {
      printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
             temperature / 10, temperature % 10,
             humidity / 10, humidity % 10);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  SENSORS_DEACTIVATE(dht11_sensor);
  PROCESS_END();
}

int main(void) {
  // Initialize Contiki and start processes
  contiki_init();
}