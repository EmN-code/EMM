#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "dev/dht11.h"
#include "dev/leds.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
  static struct etimer timer;
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if(dht11_read() == DHT11_ERROR) {
      printf("DHT11 error\n");
    } else {
      int temp_integer = dht11.value(DHT11_VALUE_TEMP_INTEGER);
      int humidity_integer = dht11.value(DHT11_VALUE_HUMIDITY_INTEGER);

      printf("Temperature: %d C, Humidity: %d %%\n", temp_integer, humidity_integer);

      leds_on(LEDS_RED | LEDS_GREEN | LEDS_BLUE);
      printf("LEDs turned on\n");
    }
    
    etimer_set(&timer, CLOCK_SECOND * 5);
  }

  PROCESS_END();
}