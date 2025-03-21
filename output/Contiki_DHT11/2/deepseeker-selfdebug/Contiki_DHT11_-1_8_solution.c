
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  configure(SENSORS_HW_INIT, 0);
  configure(SENSORS_ACTIVE, 1);

  // Set the timer to read the DHT11 sensor every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the sensor values
    int humidity_integer = value(DHT11_VALUE_HUMIDITY_INTEGER);
    int humidity_decimal = value(DHT11_VALUE_HUMIDITY_DECIMAL);
    int temperature_integer = value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int temperature_decimal = value(DHT11_VALUE_TEMPERATURE_DECIMAL);

    // Print the values
    printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", 
           humidity_integer, humidity_decimal, 
           temperature_integer, temperature_decimal);

    // Reset the timer
    etimer_reset(&timer);
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
