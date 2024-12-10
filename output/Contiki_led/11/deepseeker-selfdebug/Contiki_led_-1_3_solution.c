
#include "contiki.h"
#include "dht11-sensor.h"  // Include the correct header file for DHT11
#include "dev/leds.h"      // Include the header file for LED control
#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

// Define the RGB LED color combinations
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

// Process to read DHT11 sensor and control RGB LED
PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  // Set the timer to read the sensor every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the humidity and temperature from the DHT11 sensor
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print the sensor values
    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on the RGB LED (for example, turn on white LED)
    leds_set(RGB_LED_WHITE);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize the Contiki system
  contiki_init();

  // Start the process
  process_start(&dht11_rgb_led_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
