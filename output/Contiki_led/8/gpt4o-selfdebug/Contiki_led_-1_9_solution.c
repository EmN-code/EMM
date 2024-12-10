#include "sensors.h"
#include "dht11-sensor.h"
#include <stdio.h>
#include <contiki.h>

PROCESS(nrf52840_dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&nrf52840_dht11_process);

PROCESS_THREAD(nrf52840_dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature_integer, humidity_integer;

  PROCESS_BEGIN();

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    SENSORS_ACTIVATE(dht11_sensor);
    temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    printf("Temperature: %d
", temperature_integer);
    printf("Humidity: %d
", humidity_integer);

    // Simulate turning on RGB LED
    printf("RGB LED is ON\n");

    SENSORS_DEACTIVATE(dht11_sensor);
  }

  PROCESS_END();
}