#include "contiki.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/dht11.h"

#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Control Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_init();

  while(1) {
    // Set the etimer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    // Read values from DHT11
    if(dht11_read(&temperature, &humidity) == DHT11_ERROR) {
      printf("Failed to read from DHT11 sensor\n");
    } else {
      printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
    }

    // Turn on the RGB LED based on some conditions
    leds_on(LEDS_RED);
    clock_delay_usec(1000000); // delay for 1 second just to simulate LED control
    leds_off(LEDS_RED);
  }

  PROCESS_END();
}
