
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "sys/clock.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1
#define RGB_LED_PIN 2

#define DHT11_SAMPLING_RATE_SECONDS 5

static struct etimer et;

static void configure_dht11(void) {
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_ACTIVE, 1);
}

static void read_dht11(void) {
  dht11_sensor.configure(SENSORS_ACTIVE, 1);
}

static void turn_on_rgb_led(void) {
  gpio_hal_arch_pin_set_output(0, RGB_LED_PIN);
  gpio_hal_arch_set_pin(0, RGB_LED_PIN);
}

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
  PROCESS_BEGIN();

  configure_dht11();

  etimer_set(&et, CLOCK_SECOND * DHT11_SAMPLING_RATE_SECONDS);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    read_dht11();
    turn_on_rgb_led();

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&dht11_rgb_process, NULL);
  return 0;
}
