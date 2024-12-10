
#include "contiki.h"
#include "dev/dht11.h"  // Include the correct header file for DHT11
#include "dev/leds.h"   // Include the header file for LEDs
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED    (LEDS_RED)
#define RGB_LED_GREEN  (LEDS_GREEN)
#define RGB_LED_BLUE   (LEDS_BLUE)

// Define the RGB LED color to turn on
#define RGB_LED_COLOR  (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

// Define the interval for reading the DHT11 sensor
#define DHT11_READ_INTERVAL (5 * CLOCK_SECOND)

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_init();

  // Set the timer for the first reading
  etimer_set(&timer, DHT11_READ_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    if(dht11_read(&temperature, &humidity) == DHT11_STATUS_OKAY) {
      printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

      // Turn on the RGB LED
      leds_set(RGB_LED_COLOR);
    } else {
      printf("Failed to read DHT11 sensor\n");
    }

    // Reset the timer for the next reading
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  // Start the Contiki system
  contiki_init();

  // Start the process
  process_start(&dht11_led_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
