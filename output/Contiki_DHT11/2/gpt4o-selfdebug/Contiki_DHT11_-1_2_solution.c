#include <contiki-net.h>
#include <contiki-lib.h>
#include <contiki-default-conf.h>
#include <dev/leds.h>
#include <dev/dht11-sensor.h>
#include <sys/etimer.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(dht11_sensor);

  // Set an event timer to fire every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    printf("Temperature: %d°C, Humidity: %d%%\n", temperature / 10, humidity / 10);

    etimer_reset(&timer);
  }

  PROCESS_END();
}