
#include "contiki.h"
#include "contiki-net.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"

#define LED_PORT 0
#define LED_PIN 2

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static void
rgb_led_on(void)
{
  gpio_hal_arch_pin_set_output(LED_PORT, LED_PIN);
  gpio_hal_arch_set_pin(LED_PORT, LED_PIN);
}

static void
rgb_led_off(void)
{
  gpio_hal_arch_pin_set_output(LED_PORT, LED_PIN);
  gpio_hal_arch_clear_pin(LED_PORT, LED_PIN);
}

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer et;
  static int dht11_status;

  PROCESS_BEGIN();

  // Configure DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 1);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  etimer_set(&et, CLOCK_SECOND * 5); // Set timer to 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read DHT11 sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    dht11_status = dht11_sensor.status(0);

    if(dht11_status == DHT11_STATUS_OKAY) {
      rgb_led_on(); // Turn on the LED if reading is successful
    } else {
      rgb_led_off(); // Turn off the LED if reading fails
    }

    etimer_reset(&et); // Reset the timer
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&dht11_led_process, NULL);
  return 0;
}
