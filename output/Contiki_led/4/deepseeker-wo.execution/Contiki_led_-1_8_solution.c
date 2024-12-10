
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"

#define LED_PORT 0
#define LED_PIN 1

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

void configure_dht11(void) {
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
}

void read_dht11_and_toggle_led(void) {
  dht11_sensor.configure(SENSORS_ACTIVE, 1);
  if (dht11_sensor.status(0) == DHT11_STATUS_OKAY) {
    gpio_hal_arch_pin_set_output(LED_PORT, LED_PIN);
    gpio_hal_arch_set_pin(LED_PORT, LED_PIN);
  } else {
    gpio_hal_arch_clear_pin(LED_PORT, LED_PIN);
  }
}

int main(void) {
  configure_dht11();
  gpio_hal_arch_pin_set_output(LED_PORT, LED_PIN);
  gpio_hal_arch_clear_pin(LED_PORT, LED_PIN);

  etimer_set(&read_timer, READ_INTERVAL);

  while (1) {
    if (etimer_expired(&read_timer)) {
      read_dht11_and_toggle_led();
      etimer_reset(&read_timer);
    }
  }

  return 0;
}
