
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "dht11-sensor.h"

#define LED_PIN 23
#define DHT11_PIN 22
#define DHT11_PORT 0

PROCESS(dht11_led_process, "DHT11 LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer et;
  static int status;

  PROCESS_BEGIN();

  // Configure GPIO for LED
  gpio_hal_pin_set_output(DHT11_PORT, LED_PIN);
  gpio_hal_clear_pin(DHT11_PORT, LED_PIN);

  // Configure DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  etimer_set(&et, CLOCK_SECOND * 5); // Set timer to 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read DHT11 sensor
    status = dht11_sensor.status(0);

    if(status == DHT11_STATUS_OKAY) {
      gpio_hal_set_pin(DHT11_PORT, LED_PIN); // Turn on LED
    } else {
      gpio_hal_clear_pin(DHT11_PORT, LED_PIN); // Turn off LED
    }

    etimer_reset(&et); // Reset timer
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&dht11_led_process, NULL);
  return 0;
}
