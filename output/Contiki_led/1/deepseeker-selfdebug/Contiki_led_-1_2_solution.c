
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1

#define RGB_LED_RED    (LEDS_RED)
#define RGB_LED_GREEN  (LEDS_GREEN)
#define RGB_LED_BLUE   (LEDS_BLUE)

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Configure DHT11 GPIO port and pin
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  // Set the timer to 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor data
    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print the sensor data
    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on the RGB LED
    leds_set(RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE);

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

  // Enter the main loop
  while(1) {
    process_run();
  }

  return 0;
}
