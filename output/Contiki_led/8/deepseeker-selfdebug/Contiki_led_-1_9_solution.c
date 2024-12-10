
#include "contiki.h"
#include "dev/dht11-sensor.h" // Ensure this header file is available
#include "dev/leds.h"
#include <stdio.h>

// Define the RGB LED control
#define RGB_LED_RED   1
#define RGB_LED_GREEN 2
#define RGB_LED_BLUE  4

// Process to read DHT11 sensor and control RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
  static struct etimer timer;
  uint8_t temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);

  // Set the timer to 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    if(dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature) == DHT11_STATUS_OKAY &&
       dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity) == DHT11_STATUS_OKAY) {
      printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

      // Turn on the RGB LED (example: turn on red LED)
      leds_on(RGB_LED_RED);
    } else {
      printf("Failed to read DHT11 sensor\n");
    }

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}
