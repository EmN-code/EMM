
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_WHITE  (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

static struct etimer timer;

void rgb_led_set(uint8_t colour) {
  leds_single_off(LEDS_ALL);
  leds_single_on(colour);
}

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor values
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on RGB LED
    rgb_led_set(RGB_LED_WHITE);
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize Contiki
  contiki_init();

  // Start the process
  process_start(&dht11_rgb_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
