#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Simulated DHT11 reading
    int temperature = 25; // Example temperature
    int humidity = 60;    // Example humidity

    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    
    // Simulate turning on RGB LED based on conditions
    if (temperature > 24) {
      leds_on(LEDS_RED);
    } else {
      leds_off(LEDS_RED);
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}