
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

static void configure_dht11(void) {
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);
}

static void read_dht11(void) {
  dht11_sensor.configure(SENSORS_ACTIVE, 1);
  int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
  int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
  printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
}

static void toggle_rgb_led(void) {
  static uint8_t led_state = 0;
  led_state ^= RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE;
  leds_set(led_state);
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
  static struct etimer timer;

  PROCESS_BEGIN();

  configure_dht11();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    read_dht11();
    toggle_rgb_led();

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&dht11_rgb_led_process, NULL);
  return 0;
}
