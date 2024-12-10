#include "contiki.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
    SENSORS_ACTIVATE(dht11_sensor);

    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    if(temperature != -1 && humidity != -1) {
      printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
    } else {
      printf("Failed to read sensor\n");
    }

    SENSORS_DEACTIVATE(dht11_sensor);
    etimer_reset(&timer);
  }

  PROCESS_END();
}

/* Configuration of the DHT11 Pin */
#define DHT11_SENSOR_PORT_BASE    NRF_P0_BASE
#define DHT11_SENSOR_PIN_MASK     (1 << 2)
