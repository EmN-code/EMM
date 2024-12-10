
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h"  // Ensure this header file is available
#include <stdio.h>

// Define the GPIO pin for the DHT11 sensor
#define DHT11_GPIO_PIN 2

// Process to read DHT11 sensor
PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  while(1) {
    // Set a timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    if(dht11_sensor.status() == DHT11_STATUS_OKAY) {
      humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
      temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
      printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
    } else {
      printf("Failed to read DHT11 sensor\n");
    }
  }

  PROCESS_END();
}

int main(void)
{
  // Start the Contiki system
  contiki_init();

  // Start the DHT11 read process
  process_start(&dht11_read_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
