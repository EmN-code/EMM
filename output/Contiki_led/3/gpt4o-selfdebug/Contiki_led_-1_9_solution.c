#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(read_dht11_process, "DHT11 Reader");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature;
  static int humidity;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read from DHT11 (mock implementation)
    // Actual function would be something like: dht11_read(&temperature, &humidity);
    temperature = 25; // mock temperature
    humidity = 60;    // mock humidity

    printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

    if(temperature > 24) { // Example condition
      leds_on(LEDS_RED);
      leds_on(LEDS_GREEN);
      leds_on(LEDS_BLUE);
    } else {
      leds_off(LEDS_RED);
      leds_off(LEDS_GREEN);
      leds_off(LEDS_BLUE);
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}