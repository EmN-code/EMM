
#include "contiki.h"
#include "contiki-net.h"
#include "dev/gpio-hal.h"
#include "contiki-default-conf.h"
#include "dht11-sensor.h"

#define LED_PORT 0
#define LED_PIN 1

#define DHT11_PORT 0
#define DHT11_PIN 2

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static void rgb_led_set(uint8_t colour) {
  gpio_hal_arch_pin_set_output(LED_PORT, LED_PIN);
  gpio_hal_arch_set_pin(LED_PORT, LED_PIN);
  gpio_hal_arch_pin_set_input(LED_PORT, LED_PIN);
}

PROCESS_THREAD(dht11_led_process, ev, data) {
  static struct etimer timer;
  static int humidity;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(dht11_sensor);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_PIN);

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    if(humidity != -1) {
      rgb_led_set(1); // Turn on the LED
    } else {
      rgb_led_set(0); // Turn off the LED
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  process_start(&dht11_led_process, NULL);
  return 0;
}
