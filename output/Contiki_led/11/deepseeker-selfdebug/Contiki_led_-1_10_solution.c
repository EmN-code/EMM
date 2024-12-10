
#include "contiki.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

// Process to read DHT11 sensor and turn on RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

  // Set the timer to 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print the values
    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on the RGB LED (e.g., turn on red LED)
    leds_set(RGB_LED_RED);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  // Start the Contiki system
  contiki_init();

  // Start the process
  process_start(&dht11_rgb_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
