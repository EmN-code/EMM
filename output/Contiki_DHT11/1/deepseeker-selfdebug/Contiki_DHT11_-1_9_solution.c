
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/gpio-hal.h"
#include "dev/dht11.h"
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(2)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
  static uint8_t checksum;

  PROCESS_BEGIN();

  // Initialize GPIO pin for DHT11
  gpio_hal_set_output(DHT11_PIN);
  gpio_hal_set(DHT11_PIN);
  etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 data
    if(dht11_read(DHT11_PIN, &humidity_integer, &humidity_decimal, &temperature_integer, &temperature_decimal, &checksum) == DHT11_STATUS_OK) {
      printf("Humidity: %d.%d %%\n", humidity_integer, humidity_decimal);
      printf("Temperature: %d.%d C\n", temperature_integer, temperature_decimal);
    } else {
      printf("Failed to read DHT11 sensor\n");
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}
