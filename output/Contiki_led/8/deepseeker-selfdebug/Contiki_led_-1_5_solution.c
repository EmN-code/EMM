
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
  static struct etimer et;
  static int temp_integer, temp_decimal, hum_integer, hum_decimal;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0); // Replace 0 with actual GPIO port
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);  // Replace 1 with actual GPIO pin
  dht11_sensor.configure(SENSORS_HW_INIT, 1);

  while(1) {
    // Set timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read DHT11 sensor values
    temp_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    temp_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
    hum_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    hum_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

    // Turn on RGB LED
    leds_set(LEDS_RED); // Assuming LEDS_RED is defined in leds.h

    // Reset the timer
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void)
{
  // Start the Contiki system
  process_start(&dht11_rgb_process, NULL);
  return 0;
}
