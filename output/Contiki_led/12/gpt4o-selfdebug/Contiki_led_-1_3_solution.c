// This code demonstrates reading data from a DHT11 sensor every five seconds
// using Contiki OS on an NRF52840 and controlling an RGB LED based on the data.

// Import necessary headers in descending alphabetical order by their first letter
#include "sys/rtimer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"
#include "contiki.h"

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

static struct etimer timer;

PROCESS_THREAD(dht11_reader_process, ev, data)
{
  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  while (1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    // Read humidity and temperature from the DHT11 sensor
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    // Check if the values are valid
    if (temperature != -1 && humidity != -1) {
      printf("Temperature: %d°C
", temperature);
      printf("Humidity: %d%%
", humidity);

      // Example: Turn on the LED in a specific color based on the temperature
      if (temperature >= 25) {
        leds_on(LEDS_RED);
      } else if (temperature < 25 && temperature >= 15) {
        leds_on(LEDS_GREEN);
      } else {
        leds_on(LEDS_BLUE);
      }
    } else {
      printf("Failed to read from DHT11 sensor
");
    }

    // Turn off the LEDs after checking
    leds_off(LEDS_RED | LEDS_GREEN | LEDS_BLUE);
  }

  // Deactivate the sensor
  SENSORS_DEACTIVATE(dht11_sensor);

  PROCESS_END();
}