#include "contiki.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  dht11_sensor.configure(SENSORS_ACTIVE, DHT11_SENSOR);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    printf("Temperature: %d
", temperature);
    printf("Humidity: %d
", humidity);
  }

  PROCESS_END();
}