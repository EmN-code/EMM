
#include "contiki.h"
#include "dht11-sensor.h"  // Ensure this header is available
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

#define LED_COLOR_ON  RGB_LED_RED  // Example color, adjust as needed
#define LED_COLOR_OFF 0

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Initialize DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  // Set timer for 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor data
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print sensor data
    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on the LED
    leds_set(LED_COLOR_ON);

    // Wait for a short period to see the LED
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Turn off the LED
    leds_set(LED_COLOR_OFF);

    // Reset timer for the next 5 seconds
    etimer_reset(&timer);
  }

  PROCESS_END();
}
