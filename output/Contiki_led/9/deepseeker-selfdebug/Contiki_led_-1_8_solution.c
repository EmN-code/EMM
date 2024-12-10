
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include <stdio.h>

#define LED_PIN 22  // Example GPIO pin for the RGB LED

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);  // Example GPIO port
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);   // Example GPIO pin
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  // Configure the LED pin
  gpio_hal_pin_set_output(0, LED_PIN);  // Example GPIO port and pin

  etimer_set(&timer, CLOCK_SECOND * 5);  // Set timer to 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Turn on the RGB LED
    gpio_hal_set_pin(0, LED_PIN);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  // Initialize the Contiki system
  process_init();
  process_start(&etimer_process, NULL);
  process_start(&dht11_led_process, NULL);

  // Main loop
  while(1) {
    process_run();
  }

  return 0;
}
