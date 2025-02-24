
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  // Set the timer to read the sensor every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the sensor values
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Print the sensor values
    printf("Humidity: %d.%d %% Temperature: %d.%d C\n",
           dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER),
           dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL),
           dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER),
           dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL));

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  platform_init();
  return 0;
}
