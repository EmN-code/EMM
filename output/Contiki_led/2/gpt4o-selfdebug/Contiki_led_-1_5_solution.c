#include "contiki.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 LED Control Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static struct etimer timer;

PROCESS_THREAD(dht11_led_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Dummy read DHT11 sensor (Replace with actual reading logic)
    int temperature = 25; // Placeholder value
    int humidity = 60;    // Placeholder value
    printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

    // Turn on the RGB LED (Replace with actual LED control logic)
    printf("Turning on the RGB LED\n");

    etimer_reset(&timer);
  }

  PROCESS_END();
}