#include "contiki.h"
#include "dev/dht11/dht11.h"
#include <stdio.h>

#define GPIO_PIN 2

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if(dht11_read(GPIO_PIN, &temperature, &humidity) == DHT11_SUCCESS) {
      printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}

int main() {
  return 0;
}