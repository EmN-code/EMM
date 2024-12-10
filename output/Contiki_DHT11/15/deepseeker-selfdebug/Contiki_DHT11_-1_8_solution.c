
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  // Set the timer to read the sensor every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the sensor values
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print the values
    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize Contiki
  contiki_init();

  // Run the process
  PROCESS_RUN();

  return 0;
}
