
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

#define RGB_LED_RED     (1 << 0)
#define RGB_LED_GREEN   (1 << 1)
#define RGB_LED_BLUE    (1 << 2)

#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

void rgb_led_set(uint8_t colour) {
  leds_off(LEDS_ALL);
  leds_on(colour);
}

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
  static struct etimer et;
  static int humidity, temperature;

  PROCESS_BEGIN();

  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if(dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER, &humidity) == DHT11_STATUS_OKAY &&
       dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER, &temperature) == DHT11_STATUS_OKAY) {
      printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

      if(humidity > 50) {
        rgb_led_set(RGB_LED_YELLOW);
      } else if(temperature > 25) {
        rgb_led_set(RGB_LED_RED);
      } else {
        rgb_led_set(RGB_LED_GREEN);
      }
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}
