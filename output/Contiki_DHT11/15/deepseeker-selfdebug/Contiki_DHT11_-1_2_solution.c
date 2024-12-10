
#include "contiki.h"
#include "dev/dht11.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

// Define the GPIO pin for the DHT11 sensor
#define DHT11_GPIO_PIN 2

// Process to read DHT11 sensor
PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int humidity_int, humidity_dec, temperature_int, temperature_dec;
  static int status;

  PROCESS_BEGIN();

  // Configure the GPIO pin for the DHT11 sensor
  gpio_hal_set_input(DHT11_GPIO_PIN);

  while(1) {
    // Set a timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    status = dht11_read(DHT11_GPIO_PIN, &humidity_int, &humidity_dec, &temperature_int, &temperature_dec);

    if(status == DHT11_STATUS_OKAY) {
      printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_int, humidity_dec, temperature_int, temperature_dec);
    } else {
      printf("Failed to read DHT11 sensor, status: %d\n", status);
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
