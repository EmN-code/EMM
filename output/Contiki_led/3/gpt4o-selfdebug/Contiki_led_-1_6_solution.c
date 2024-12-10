#include "contiki.h"
#include "dev/dht11.h"
#include "dev/leds.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();
  
  dht11_init();

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    int temperature, humidity;
    if(dht11_read(&temperature, &humidity) != -1) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
      leds_on(LEDS_NUM_TO_MASK(LEDS_RED | LEDS_GREEN | LEDS_BLUE));
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}