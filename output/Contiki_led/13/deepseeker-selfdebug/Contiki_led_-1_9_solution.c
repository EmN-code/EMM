
#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

// Simulated DHT11 sensor reading
#define SIMULATED_DHT11_TEMPERATURE 25
#define SIMULATED_DHT11_HUMIDITY 60

// RGB LED control
#define RGB_LED_RED   1
#define RGB_LED_GREEN 2
#define RGB_LED_BLUE  4

PROCESS(dht11_sensor_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_sensor_process);

PROCESS_THREAD(dht11_sensor_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the timer for 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Simulate reading the DHT11 sensor
    temperature = SIMULATED_DHT11_TEMPERATURE;
    humidity = SIMULATED_DHT11_HUMIDITY;

    // Print the sensor values
    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

    // Turn on the RGB LED (e.g., turn on the red LED)
    leds_set(RGB_LED_RED);

    // Reset the timer for the next reading
    etimer_reset(&timer);
  }

  PROCESS_END();
}
