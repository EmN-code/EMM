
#include "contiki.h"
#include "dev/dht11-sensor.h" // Ensure this header file is available
#include "dev/leds.h"
#include <stdio.h>

// Define the RGB LED pins (example for NRF52840)
#define RED_LED_PIN   22
#define GREEN_LED_PIN 23
#define BLUE_LED_PIN  24

// Process to read DHT11 sensor and turn on RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);

  // Set the timer to 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print the sensor values
    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on the RGB LED (example: turn on red LED)
    leds_single_on(LEDS_RED);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}
