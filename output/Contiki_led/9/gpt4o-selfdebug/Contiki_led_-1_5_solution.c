#include "contiki.h"
#include "lib/sensors.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Control");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    printf("Reading DHT11 sensor\n");
    // Implementation depends on available library

    /* Simulate turning on the RGB LED */
    leds_on(LEDS_RED);
    leds_off(LEDS_GREEN);
    leds_off(LEDS_BLUE);
    printf("RGB LED is on\n");

    etimer_reset(&timer);
  }

  PROCESS_END();
}

void main(void) {
  process_start(&dht11_led_process, NULL);
}