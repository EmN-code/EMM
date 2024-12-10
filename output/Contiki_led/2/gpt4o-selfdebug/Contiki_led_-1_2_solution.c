#include <stdio.h>
#include <contiki.h>
#include <dev/leds.h> // Ensure you have a proper LED driver
#include "dev/dht11.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize timer for 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor data
    if(dht11_read(&temperature, &humidity) != DHT11_ERROR) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

      // Control RGB LED based on temperature
      if(temperature > 25) {
        leds_on(LEDS_RED);
        leds_off(LEDS_GREEN);
        leds_off(LEDS_BLUE);
      } else {
        leds_on(LEDS_GREEN);
        leds_off(LEDS_RED);
        leds_off(LEDS_BLUE);
      }
    }

    // Reset the timer for next reading
    etimer_reset(&timer);
  }

  PROCESS_END();
}