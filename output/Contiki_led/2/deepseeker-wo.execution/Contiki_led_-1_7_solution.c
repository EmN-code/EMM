
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "dht11-sensor.h"

#define LED_PIN GPIO_PIN(0, 13) // Example pin for RGB LED
#define DHT11_PIN GPIO_PIN(0, 15) // Example pin for DHT11 sensor

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer et;
  static int dht11_status;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_PORT_TO_BASE(0));
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  // Configure LED pin
  gpio_hal_pin_set_output(GPIO_PORT_TO_BASE(0), LED_PIN);

  while(1) {
    // Set timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read DHT11 sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    dht11_status = dht11_sensor.status(0);

    if(dht11_status == DHT11_STATUS_OKAY) {
      // Turn on the LED
      gpio_hal_set_pin(GPIO_PORT_TO_BASE(0), LED_PIN);
    } else {
      // Turn off the LED
      gpio_hal_clear_pin(GPIO_PORT_TO_BASE(0), LED_PIN);
    }

    // Reset the timer
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

  // Run the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
