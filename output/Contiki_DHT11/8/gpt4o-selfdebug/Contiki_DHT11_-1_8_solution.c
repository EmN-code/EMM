#include "sensors.h"
#include "dev/gpio-hal.h"
#include "contiki.h"
#include "dht11-sensor.h"

#include <stdio.h>

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int temperature_integer;
  static int humidity_integer;

  PROCESS_BEGIN();

  // Configure the DHT11 sensor on GPIO port and pin
  SENSORS_ACTIVATE(dht11_sensor);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, 5 * CLOCK_SECOND);

    // Wait until timer expires
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read temperature and humidity
    dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    // Print the obtained values
    printf("Temperature: %d\n", temperature_integer);
    printf("Humidity: %d\n", humidity_integer);
  }

  PROCESS_END();
}