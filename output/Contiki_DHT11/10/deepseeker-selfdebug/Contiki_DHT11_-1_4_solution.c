
#include "contiki.h"
#include "dev/dht11.h"  // Ensure this header file is available
#include "dev/gpio-hal.h"
#include <stdio.h>

// Define the GPIO pin for the DHT11 sensor
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Configure the GPIO pin for the DHT11 sensor
  gpio_hal_set_input(DHT11_GPIO_PIN);

  while(1) {
    // Set a timer to read the DHT11 sensor every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor data
    if(dht11_read(DHT11_GPIO_PIN, &humidity, &temperature) == DHT11_STATUS_OKAY) {
      printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
    } else {
      printf("Failed to read DHT11 sensor\n");
    }
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize the Contiki system
  contiki_init();

  // Start the DHT11 read process
  process_start(&dht11_read_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
