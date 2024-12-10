#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_sensor_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_sensor_process);

PROCESS_THREAD(dht11_sensor_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();
  
  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

    if (temperature != -1 && humidity != -1) {
        leds_on(LEDS_RED);
    }
    else {
        leds_off(LEDS_RED);
    }
  }
  
  SENSORS_DEACTIVATE(dht11_sensor);
  PROCESS_END();
}