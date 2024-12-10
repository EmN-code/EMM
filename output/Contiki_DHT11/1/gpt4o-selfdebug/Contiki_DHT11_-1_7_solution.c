#include "contiki.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

#define DHT11_PIN 2

PROCESS_THREAD(dht11_process, ev, data) {
  static struct etimer timer;
  PROCESS_BEGIN();

  // Set the GPIO pin to input mode
  gpio_hal_arch_port_pin_set_input(GPIO_HAL_PORT_A, DHT11_PIN);

  while(1) {
    // Read the DHT11 data
    int val = gpio_hal_arch_port_pin_get(GPIO_HAL_PORT_A, DHT11_PIN);
    printf("DHT11 Value: %d\n", val);

    // Wait for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}