
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04
#define RGB_LED_WHITE (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

static struct etimer et;

PROCESS_THREAD(dht11_led_process, ev, data)
{
  PROCESS_BEGIN();

  // Configure DHT11 sensor
  sensors_activate(&dht11_sensor);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  while(1) {
    etimer_set(&et, CLOCK_SECOND * 5); // Set timer to 5 seconds

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read DHT11 sensor
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    // Turn on RGB LED
    leds_set(RGB_LED_WHITE);

    etimer_reset(&et);
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
