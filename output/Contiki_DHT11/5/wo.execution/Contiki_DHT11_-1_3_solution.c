#define BUTTON_HAL_GET_DESCRIPTION(b)
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "sensors.h"
#include "sys/etimer.h"
#include "dht11-sensor.h"
#include "contiki-net.h"
#include "contiki.h"

#include <stdio.h>

// Declare process
PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  // Define timer
  static struct etimer timer;

  // Begin process
  PROCESS_BEGIN();

  // Configure DHT11 GPIO port and pin
  SENSORS_ACTIVATE(dht11_sensor);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

  while(1) {
    // Set timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait until timer expires
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 values
    int temperature_integral = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
    int humidity_integral = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

    // Print the sensor values
    printf("Temperature: %d.%d C\n", temperature_integral, temperature_decimal);
    printf("Humidity: %d.%d %%\n", humidity_integral, humidity_decimal);
  }

  PROCESS_END();
}