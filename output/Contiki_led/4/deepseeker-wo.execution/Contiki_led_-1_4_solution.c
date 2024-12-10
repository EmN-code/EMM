
#include "contiki.h"
#include "contiki-net.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1
#define RGB_LED_PIN 2

static struct etimer et;

void rgb_led_set(uint8_t colour) {
  gpio_hal_arch_pin_set_output(DHT11_GPIO_PORT, RGB_LED_PIN);
  gpio_hal_arch_set_pin(DHT11_GPIO_PORT, RGB_LED_PIN);
}

void read_dht11() {
  static uint8_t dht11_data[5];
  gpio_hal_arch_pin_set_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  gpio_hal_arch_clear_pin(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  clock_delay_usec(18000);
  gpio_hal_arch_set_pin(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  gpio_hal_arch_pin_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);

  // Simulate reading DHT11 data
  dht11_data[0] = 50; // Humidity integer
  dht11_data[1] = 0;  // Humidity decimal
  dht11_data[2] = 25; // Temperature integer
  dht11_data[3] = 0;  // Temperature decimal
  dht11_data[4] = dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]; // Checksum

  // Turn on RGB LED
  rgb_led_set(1);
}

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
  PROCESS_BEGIN();

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    read_dht11();
  }

  PROCESS_END();
}

int main() {
  process_start(&dht11_process, NULL);
  return 0;
}
