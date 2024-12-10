#include "dev/dht11.h"
#include "contiki.h"

PROCESS(dht11_reading_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_reading_process);

PROCESS_THREAD(dht11_reading_process, ev, data) {

  static struct etimer timer;
  static int16_t temperature, humidity;

  PROCESS_BEGIN();

  // Set up a periodic timer that expires every five seconds.
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read values from the DHT11 sensor
    if (dht11_read(&temperature, &humidity) != DHT11_STATUS_OKAY) {
      printf("Error reading from DHT11 sensor\n");
    } else {
      printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", 
              temperature / 10, temperature % 10,
              humidity / 10, humidity % 10);
    }

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}