#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(read_dht11_process, "Read DHT11 process");
AUTOSTART_PROCESSES(&read_dht11_process);

static struct etimer timer;

PROCESS_THREAD(read_dht11_process, ev, data)
{
  PROCESS_BEGIN();

  // Configure the DHT11 sensor GPIO port and pin
  SENSORS_ACTIVATE(dht11_sensor);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait until the timer expires
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read temperature and humidity integer values
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    // Print the values to the console
    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
  }

  PROCESS_END();
}