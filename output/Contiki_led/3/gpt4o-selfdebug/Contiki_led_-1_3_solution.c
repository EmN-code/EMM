#include "contiki.h"
#include <stdio.h>
#include "dev/dht11.h"
#include "dev/leds.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  static uint8_t temperature, humidity;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    if(dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
      printf("Temperature: %u, Humidity: %u\n", temperature, humidity);

      if(temperature > 25) {
        // Turn on RGB LED as an indicator
        leds_on(LEDS_RED);
      } else {
        leds_off(LEDS_RED);
      }
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}