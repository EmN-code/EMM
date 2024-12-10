
#include "contiki.h"
#include "sys/etimer.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int humidity_integer, temperature_integer;

  PROCESS_BEGIN();

  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_ACTIVE, 1);

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  process_start(&dht11_read_process, NULL);
  return 0;
}
