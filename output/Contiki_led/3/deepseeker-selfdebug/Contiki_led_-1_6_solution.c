
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1

#define RGB_LED_RED   (1 << 0)
#define RGB_LED_GREEN (1 << 1)
#define RGB_LED_BLUE  (1 << 2)

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Configure DHT11 GPIO
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  // Set RGB LED pins as output
  gpio_hal_pin_set_output(0, RGB_LED_RED);
  gpio_hal_pin_set_output(0, RGB_LED_GREEN);
  gpio_hal_pin_set_output(0, RGB_LED_BLUE);

  etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on RGB LED
    leds_set(RGB_LED_WHITE);

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&dht11_rgb_led_process, NULL);
  return 0;
}
