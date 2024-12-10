
#include "contiki.h"
#include "sys/etimer.h"
#include "sys/clock.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer et;
  static int humidity_integer, temperature_integer;

  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_ACTIVE, 1);

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read the sensor values
    humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print the values (Note: printf is not allowed, so we use a placeholder function)
    // Replace the following line with your logging or output mechanism
    // printf("Humidity: %d, Temperature: %d\n", humidity_integer, temperature_integer);

    // Reset the timer
    etimer_reset(&et);
  }

  PROCESS_END();
}
