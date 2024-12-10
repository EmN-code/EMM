#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <contiki.h>
#include "dev/rgb-led/rgb-led.h"
#include "dev/dht11/dht11.h"
#include "sys/etimer.h"

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;
  PROCESS_BEGIN();

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    if (dht11_read(&temperature, &humidity) == DHT11_OK) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

      // Turn on RGB LED if successfully read
      rgb_led_on();
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}