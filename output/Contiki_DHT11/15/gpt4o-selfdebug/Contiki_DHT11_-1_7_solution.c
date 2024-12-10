#include "sensors.h"
#include "contiki.h"

PROCESS(dht11_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature_integer;
  static int humidity_integer;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    SENSORS_ACTIVATE(dht11);
    
    temperature_integer = dht11.value(DHT11_SENSOR_TEMP) / 10;
    humidity_integer = dht11.value(DHT11_SENSOR_HUMIDITY) / 10;
    
    printf("Temperature: %d°C
", temperature_integer);
    printf("Humidity: %d%%
", humidity_integer);

    SENSORS_DEACTIVATE(dht11);

    etimer_reset(&timer);
  }

  PROCESS_END();
}