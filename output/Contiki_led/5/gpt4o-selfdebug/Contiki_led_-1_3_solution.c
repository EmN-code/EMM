/* The following code demonstrates how to read from a DHT11 sensor and control an RGB LED using Contiki OS on an NRF52840. */

/* Import the libraries in descending alphabetical order by their first letter */
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki.h"
#include <stdio.h>
#include "dev/leds.h"

// Define the simulation period
#define DHT11_READ_INTERVAL (5 * CLOCK_SECOND)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer; // Event timer
  PROCESS_BEGIN();

  // Initialize the timer
  etimer_set(&timer, DHT11_READ_INTERVAL);

  while(1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Simulate reading the DHT11 sensor
    printf("Reading from DHT11 sensor...\n");
    // Simulate values for demonstration purposes
    int temperature = 25; // Assume a temperature value
    int humidity = 60;    // Assume a humidity value

    // Turn on the RGB LED if temperature is above 20 degrees
    if (temperature > 20) {
      printf("Temperature is %d°C, turning on RGB LED...\n", temperature);
      leds_on(LEDS_RED); // Turn on the red LED in this example
      leds_on(LEDS_GREEN);
      leds_on(LEDS_BLUE);
    }

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}