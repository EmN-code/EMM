
#include "contiki.h"
#include "dev/dht11-sensor.h"  // Ensure this header file is included
#include "dev/leds.h"
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_WHITE   (LEDS_RED | LEDS_GREEN | LEDS_BLUE)
#define RGB_LED_YELLOW  (LEDS_RED | LEDS_GREEN)
#define RGB_LED_MAGENTA (LEDS_RED | LEDS_BLUE)
#define RGB_LED_CYAN    (LEDS_GREEN | LEDS_BLUE)

// Define the period for reading the DHT11 sensor
#define READ_PERIOD (5 * CLOCK_SECOND)

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);

  // Set the timer for 5 seconds
  etimer_set(&timer, READ_PERIOD);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print the readings
    printf("Humidity: %d %%\n", humidity);
    printf("Temperature: %d C\n", temperature);

    // Turn on the RGB LED
    leds_set(RGB_LED_WHITE);

    // Reset the timer for the next reading
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  // Start the Contiki system
  contiki_init();

  // Start the DHT11 reader process
  process_start(&dht11_reader_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    PROCESS_YIELD();
  }

  return 0;
}
