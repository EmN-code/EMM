
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11.h"  // Ensure this header is available or replace with the correct one
#include <stdio.h>

#define RGB_LED_RED    0x01
#define RGB_LED_GREEN  0x02
#define RGB_LED_BLUE   0x04

#define RGB_LED_CYAN   (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_WHITE  (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)

#define DHT11_STATUS_OKAY (0)

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

void rgb_led_off(void) {
  leds_off(LEDS_ALL);
}

void rgb_led_on(uint8_t color) {
  leds_set(color);
}

PROCESS_THREAD(dht11_reader_process, ev, data) {
  static struct etimer timer;
  static int8_t temperature;
  static int8_t humidity;
  static uint8_t status;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    status = dht11_read(&temperature, &humidity);

    if(status == DHT11_STATUS_OKAY) {
      printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
      rgb_led_on(RGB_LED_WHITE);  // Turn on the RGB LED
    } else {
      printf("Failed to read DHT11 sensor\n");
      rgb_led_off();  // Turn off the RGB LED
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void) {
  return 0;
}
