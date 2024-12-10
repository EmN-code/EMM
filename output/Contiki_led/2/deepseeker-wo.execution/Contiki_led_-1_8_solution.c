
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "dht11-sensor.h"

#define LED_PIN 2
#define DHT11_PIN 1

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer et;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_PIN);
  dht11_sensor.configure(SENSORS_ACTIVE, 1);

  // Configure LED pin
  gpio_hal_arch_pin_set_output(0, LED_PIN);

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 5); // Set timer for 5 seconds

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read DHT11 sensor
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Turn on LED
    gpio_hal_arch_set_pin(0, LED_PIN);

    // Wait for a short period to keep the LED on
    etimer_set(&et, CLOCK_SECOND / 10); // 100ms
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Turn off LED
    gpio_hal_arch_clear_pin(0, LED_PIN);
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&dht11_led_process, NULL);
  return 0;
}
