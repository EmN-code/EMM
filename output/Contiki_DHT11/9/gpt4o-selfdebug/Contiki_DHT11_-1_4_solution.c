#include "contiki-net.h"
#include "contiki.h"
#include <stdio.h>

#define DHT11_PIN 2

PROCESS(dht11_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  // Initialize the etimer to trigger every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    printf("Reading DHT11 on GPIO pin %d\n", DHT11_PIN);
    // Code to read from DHT11 would go here

    // Restart the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}