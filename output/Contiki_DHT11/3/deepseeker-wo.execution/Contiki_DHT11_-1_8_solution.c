
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "sys/log.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2
#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

void dht11_init(void) {
  gpio_hal_pin_set_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  gpio_hal_clear_pin(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  clock_delay(18 * CLOCK_SECOND);
  gpio_hal_set_pin(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  gpio_hal_pin_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

void dht11_read(void) {
  // Simulated DHT11 read function
  LOG_INFO("DHT11 read\n");
}

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
  PROCESS_BEGIN();

  dht11_init();

  etimer_set(&read_timer, READ_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));

    dht11_read();

    etimer_reset(&read_timer);
  }

  PROCESS_END();
}

int main(void) {
  process_start(&dht11_read_process, NULL);
  return 0;
}
