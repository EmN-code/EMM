
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h" // Ensure this header is available or replace with appropriate sensor driver

#include <stdio.h>

#define RGB_LED_RED    (1 << 0)
#define RGB_LED_GREEN  (1 << 1)
#define RGB_LED_BLUE   (1 << 2)

#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5); // Set timer for 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor data
    if(dht11_read(&temperature, &humidity) == 0) {
      printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

      // Turn on RGB LED based on temperature
      if(temperature < 20) {
        leds_set(RGB_LED_CYAN);
      } else if(temperature >= 20 && temperature < 30) {
        leds_set(RGB_LED_GREEN);
      } else {
        leds_set(RGB_LED_RED);
      }
    } else {
      printf("Failed to read DHT11 sensor\n");
    }

    etimer_reset(&timer); // Reset the timer
  }

  PROCESS_END();
}

int main(void)
{
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
