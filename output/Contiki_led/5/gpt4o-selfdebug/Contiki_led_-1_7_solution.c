#include "contiki.h"
#include "dev/leds.h"
#include "dht11.h"

PROCESS(dht11_process, "DHT11 process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if(dht11_read(DHT11_PIN) == DHT11_STATUS_OKAY) {
      temperature = dht11_value(DHT11_VALUE_TEMPERATURE_INTEGER);
      humidity = dht11_value(DHT11_VALUE_HUMIDITY_INTEGER);

      leds_on(LEDS_RED);

      printf("Temperature: %d C, Humidity: %d\n", temperature, humidity);
    } else {
      printf("Failed to read from sensor\n");
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&dht11_process, NULL);
  while(1) {}  // keep the process running
  return 0;
}