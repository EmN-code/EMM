
#include "contiki.h"
#include "dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  sensors_activate(&dht11_sensor);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0); // GPIO Port 0
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);  // GPIO Pin 2

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor values
    int humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
    int temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);

    printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", 
           humidity_integer, humidity_decimal, 
           temperature_integer, temperature_decimal);
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize Contiki
  contiki_init();

  // Start the process
  process_start(&dht11_read_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
