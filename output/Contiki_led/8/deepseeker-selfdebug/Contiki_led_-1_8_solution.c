
#include "contiki.h"
#include "dev/dht11-sensor.h" // Ensure this header file is available
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  // Set the timer to 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print the readings
    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on the RGB LED
    leds_on(LEDS_RED);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  // Initialize Contiki
  contiki_init();

  // Start the process
  process_start(&dht11_led_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
