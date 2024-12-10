
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/leds.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

#define LED_PIN GPIO_PIN(0, 13) // Example GPIO pin for LED

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_PORT_TO_BASE(GPIO_PORT_0));
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_PIN_TO_MASK(GPIO_PIN_15)); // Example GPIO pin for DHT11

  // Configure LED pin
  gpio_hal_arch_pin_set_output(GPIO_PORT_0, LED_PIN);
  gpio_hal_arch_clear_pin(GPIO_PORT_0, LED_PIN);

  etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on LED
    gpio_hal_arch_set_pin(GPIO_PORT_0, LED_PIN);

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&dht11_led_process, NULL);
  return 0;
}
