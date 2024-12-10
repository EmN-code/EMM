
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  // Configure DHT11 GPIO
  gpio_hal_arch_pin_set_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  gpio_hal_arch_set_pin(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  gpio_hal_arch_clear_pin(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  gpio_hal_arch_pin_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);

  // Initialize the sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  etimer_set(&timer, CLOCK_SECOND * 5); // Set timer to 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    if(dht11_sensor.status(0) == DHT11_STATUS_OKAY) {
      printf("Humidity: %d.%d %% Temperature: %d.%d C\n",
             dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER),
             dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL),
             dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER),
             dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL));
    } else {
      printf("Failed to read DHT11 sensor\n");
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize Contiki
  contiki_init();

  // Start the process
  process_start(&dht11_read_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
