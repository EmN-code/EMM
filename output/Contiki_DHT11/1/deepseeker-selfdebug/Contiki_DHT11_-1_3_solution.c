
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h"
#include <stdio.h>

#define DHT11_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_init(DHT11_PIN);

  // Set the timer to trigger every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    if(dht11_read(&temperature, &humidity) == DHT11_OK) {
      printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
      printf("Failed to read DHT11 sensor\n");
    }

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  // Initialize the Contiki system
  process_init();
  process_start(&etimer_process, NULL);
  process_start(&dht11_read_process, NULL);

  // Main loop
  while(1) {
    process_run();
  }

  return 0;
}
