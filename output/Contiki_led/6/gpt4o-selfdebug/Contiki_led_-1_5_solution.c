#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(sensor_led_process, "Sensor LED process");
AUTOSTART_PROCESSES(&sensor_led_process);

// Simulated function to read from DHT11 (normally would require specific libraries)
int read_dht11_temperature() {
  // In an actual implementation, this function would interface with the
  // DHT11 sensor and return the temperature reading.
  // Here we'll simulate it always returning 25 degrees as a placeholder.
  return 25; 
}

PROCESS_THREAD(sensor_led_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    int temperature = read_dht11_temperature();
    printf("Temperature: %d C\n", temperature);

    // Turn on the RGB LED. In an actual implementation, data would
    // need to be sent to the LED hardware via specific GPIO pins.
    leds_on(RGB_LED_WHITE);

    // Delay to simulate the LED being on for a moment
    etimer_set(&timer, CLOCK_SECOND * 1);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    leds_off(RGB_LED_WHITE);
  }
  
  PROCESS_END();
}