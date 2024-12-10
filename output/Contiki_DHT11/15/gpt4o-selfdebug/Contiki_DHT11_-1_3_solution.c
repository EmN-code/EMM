#include "sensors.h"
#include "gpio-hal.h"
#include "contiki.h"

#define DHT11_PIN 2

PROCESS(read_dht11_process, "Read DHT11 every 5 seconds");
AUTOSTART_PROCESSES(&read_dht11_process);

static const struct sensors_sensor *dht11;

PROCESS_THREAD(read_dht11_process, ev, data)
{
  static struct etimer timer;
  static int temp_integer;
  static int humidity_integer;

  PROCESS_BEGIN();

  // Locate and configure the DHT11 sensor
  dht11 = sensors_find("DHT11");

  if (!dht11) {
    printf("DHT11 sensor not found!\n");
    PROCESS_EXIT();
  }

  // Configuring GPIO, Pin 2 as input
  gpio_hal_arch_pin_set_input(DHT11_PIN, 0);

  while(1) {

    // Set a timer for 5 seconds
    etimer_set(&timer, 5 * CLOCK_SECOND);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Activate the sensor
    dht11->activate();

    // Read temperature and humidity from the DHT11
    temp_integer = dht11->value(DHT11_VALUE_TEMPERATURE_INTEGER);
    humidity_integer = dht11->value(DHT11_VALUE_HUMIDITY_INTEGER);

    // Check if data was read correctly (non-negative values indicate success)
    if (temp_integer >= 0 && humidity_integer >= 0) {
        printf("Temp: %dC, Humidity: %d\n", temp_integer, humidity_integer);
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }

    // Deactivate the sensor to save power
    dht11->deactivate();
  }

  PROCESS_END();
}

int main() {
  process_start(&read_dht11_process, NULL);
  return 0;
}