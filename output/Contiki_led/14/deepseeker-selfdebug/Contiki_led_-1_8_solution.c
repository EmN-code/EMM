
#include "contiki.h"
#include "dht11-sensor.h"  // Include the correct header file for DHT11 sensor
#include "dev/gpio-hal.h"  // Include the GPIO HAL for controlling the RGB LED

#define RGB_LED_PORT 0  // Define the GPIO port for the RGB LED
#define RGB_LED_PIN  1  // Define the GPIO pin for the RGB LED

PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data)
{
  static struct etimer timer;
  static int dht11_status;

  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);  // Set the GPIO port for DHT11
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);   // Set the GPIO pin for DHT11
  dht11_sensor.configure(SENSORS_HW_INIT, 0);            // Initialize the DHT11 sensor

  // Configure the RGB LED
  gpio_hal_arch_pin_set_output(RGB_LED_PORT, RGB_LED_PIN);  // Set the RGB LED pin as output

  etimer_set(&timer, CLOCK_SECOND * 5);  // Set the timer to 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    dht11_status = dht11_sensor.status(0);

    if(dht11_status == DHT11_STATUS_OKAY) {
      // Turn on the RGB LED
      gpio_hal_arch_set_pin(RGB_LED_PORT, RGB_LED_PIN);
    } else {
      // Turn off the RGB LED
      gpio_hal_arch_clear_pin(RGB_LED_PORT, RGB_LED_PIN);
    }

    etimer_reset(&timer);  // Reset the timer
  }

  PROCESS_END();
}

int main(void)
{
  // Start the Contiki system
  contiki_init();

  // Start the process
  process_start(&dht11_rgb_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
