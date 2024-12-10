#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/button-hal.h"
#include "dev/dht11.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_init();

  while(1) {
    // Set the event timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read temperature and humidity from DHT11
    if(dht11_read(GPIO_PIN(0, 2), &temperature, &humidity) == DHT11_SUCCESS) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    } else {
      printf("Failed to read from DHT11\n");
    }
  }

  PROCESS_END();
}