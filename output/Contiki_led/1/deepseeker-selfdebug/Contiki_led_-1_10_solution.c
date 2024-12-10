
#include "contiki.h"
#include "dev/dht11-sensor.h"  // Ensure this header file is available
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);  // Replace with actual GPIO port
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);   // Replace with actual GPIO pin

  etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer for 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    printf("Humidity: %d, Temperature: %d\n", humidity, temperature);

    // Turn on RGB LED
    leds_set(LEDS_RED);  // Assuming LEDS_RED is defined in leds.h

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize Contiki
  contiki_init();

  // Start the process
  process_start(&dht11_led_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
