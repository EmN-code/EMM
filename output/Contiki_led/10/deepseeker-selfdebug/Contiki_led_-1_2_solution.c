
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1

#define RGB_LED_RED    (1 << 0)
#define RGB_LED_GREEN  (1 << 1)
#define RGB_LED_BLUE   (1 << 2)

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  // Set timer for 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Turn on RGB LED
    leds_set(RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE);

    // Print sensor values
    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Reset timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
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
