
#include "contiki.h"
#include "dev/gpio.h"
#include "sys/etimer.h"
#include "sys/process.h"
#include "sys/clock.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PIN 2

PROCESS(dht11_reading_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_reading_process);

static struct etimer timer;

PROCESS_THREAD(dht11_reading_process, ev, data)
{
  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  while(1) {
    // Set the timer to 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    int humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
    int temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);

    // Print the values
    printf("Humidity: %d.%d %%, Temperature: %d.%d C\n",
           humidity_integer, humidity_decimal,
           temperature_integer, temperature_decimal);
  }

  PROCESS_END();
}
