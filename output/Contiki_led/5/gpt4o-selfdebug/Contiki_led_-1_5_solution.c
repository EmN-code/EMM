#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include <stdio.h>

#define RGB_LED_RED 1
#define RGB_LED_GREEN 2
#define RGB_LED_BLUE 4

#define RGB_LED_WHITE (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(nrf52840_dht11_rgb_led_process, "NRF52840 DHT11 RGB LED Process");
AUTOSTART_PROCESSES(&nrf52840_dht11_rgb_led_process);

static struct etimer timer;

PROCESS_THREAD(nrf52840_dht11_rgb_led_process, ev, data)
{
  PROCESS_BEGIN();

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if (dht11_sensor.status() == DHT11_STATUS_OKAY) {
      // Assuming functions to turn on the RGB LED are implemented
      int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
      int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

      printf("Temperature: %d C, Humidity: %d %\n", temperature, humidity);

      // Turn on the RGB LED
      // set_rgb_led(RGB_LED_WHITE); // This sets the RGB LED to white
    }
    else {
      printf("DHT11 sensor reading failed\n");
    }
  }

  PROCESS_END();
}
