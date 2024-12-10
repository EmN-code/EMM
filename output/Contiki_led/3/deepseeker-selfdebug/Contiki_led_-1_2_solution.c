
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"  // Ensure this header is available or replace with appropriate sensor driver
#include <stdio.h>

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

static void read_dht11(void) {
  uint8_t temperature, humidity;
  if (dht11_read(&temperature, &humidity) == DHT11_STATUS_OK) {
    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
    leds_set(RGB_LED_WHITE);  // Turn on the RGB LED
  } else {
    printf("Failed to read DHT11 sensor\n");
    leds_set(RGB_LED_RED);  // Turn on the RGB LED in red to indicate error
  }
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
  static struct etimer timer;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer to 5 seconds

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    read_dht11();
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void) {
  // Initialize Contiki
  contiki_init();

  // Start the process
  process_start(&dht11_rgb_led_process, NULL);

  // Enter the Contiki main loop
  while (1) {
    process_run();
  }

  return 0;
}
