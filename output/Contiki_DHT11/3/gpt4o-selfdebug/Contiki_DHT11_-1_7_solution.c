#include "contiki.h"
#include <stdio.h>
#include "dev/gpio.h"
#include "dev/leds.h"
#include "nrf_gpio.h"

#define DHT11_PIN            2      /* GPIO pin where the DHT11 is connected */
#define DHT11_READ_INTERVAL  (CLOCK_SECOND * 5)

PROCESS(dht11_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

/*---------------------------------------------------------------------------*/
static void read_dht11_sensor(void) {
  // Configure GPIO as input
  nrf_gpio_cfg_input(DHT11_PIN, NRF_GPIO_PIN_NOPULL);
  printf("Reading DHT11 sensor on pin %d\n", DHT11_PIN);
  // Add code to read from the DHT11 sensor and print the values
  // This typically involves sending a start signal and processing the response
  // Note: DHT11 read implementation is not trivial due to timing requirements
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  while(1) {
    etimer_set(&timer, DHT11_READ_INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    read_dht11_sensor();
  }

  PROCESS_END();
}