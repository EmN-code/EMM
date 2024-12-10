
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  // Configure DHT11 GPIO port and pin
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  // Initialize the sensor
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  while(1) {
    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Activate the sensor to read data
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Print the sensor data
    printf("Humidity: %d.%d %% Temperature: %d.%d C\n",
           dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER),
           dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL),
           dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER),
           dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL));
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize the Contiki system
  contiki_init();

  // Start the DHT11 read process
  process_start(&dht11_read_process, NULL);

  // Enter the main loop
  while(1) {
    process_run();
  }

  return 0;
}
