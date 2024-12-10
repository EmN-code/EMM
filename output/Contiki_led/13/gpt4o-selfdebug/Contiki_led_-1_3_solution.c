#include "net/netstack.h"
#include "dev/leds.h"
#include "contiki.h"
#include <stdio.h>

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
  static struct etimer timer;
  static int temperature;
  static int humidity;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Mock reading values from DHT11 sensor
    temperature = 25; // Example temperature value
    humidity = 60;    // Example humidity value

    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

    // Turn the RGB LED on
    leds_on(LEDS_RED);
    leds_on(LEDS_GREEN);
    leds_on(LEDS_BLUE);

    etimer_reset(&timer);
  }

  PROCESS_END();
}
