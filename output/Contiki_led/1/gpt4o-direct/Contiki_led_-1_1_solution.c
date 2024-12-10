#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"
#include <stdio.h>

PROCESS(read_dht11_and_control_led, "Read DHT11 and Control RGB LED");
AUTOSTART_PROCESSES(&read_dht11_and_control_led);

PROCESS_THREAD(read_dht11_and_control_led, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  while (1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    if (dht11_read(&temperature, &humidity) == DHT11_OK) {
      // Print the temperature and humidity values
      printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

      // Turn on the RGB LED (assuming red, green, and blue can be turned on simultaneously)
      leds_on(LEDS_RED);
      leds_on(LEDS_GREEN);
      leds_on(LEDS_BLUE);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}