#include "contiki.h"
#include "lib/sensors.h"
#include "dev/dht11-sensor.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  // Activate the sensor
  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read values from the sensor
    int hum_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
    int hum_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    // Perform further processing or print the values
    printf("Humidity: %d.%d\n", hum_int, hum_dec);
  }

  PROCESS_END();
}
