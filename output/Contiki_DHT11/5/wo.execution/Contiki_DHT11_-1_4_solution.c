#include "contiki.h"
#include "dht11-sensor.h"
#include "sys/etimer.h"
#include "sensors.h"

#ifndef GPIO_HAL_PORT_0
#define GPIO_HAL_PORT_0 0
#endif

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
  static struct etimer timer;
  int temperature, humidity;

  PROCESS_BEGIN();

  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);
  SENSORS_ACTIVATE(dht11_sensor);

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if (dht11_sensor.status(SENSORS_READY) == DHT11_STATUS_OKAY) {
      temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
      humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    } else {
      temperature = -1;
      humidity = -1;
    }

    // Use the data (temperature and humidity)

    etimer_reset(&timer);
  }

  PROCESS_END();
}