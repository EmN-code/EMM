#include "sys/pt.h"
#include "sys/etimer.h"
#include "dev/leds.h"
#include "dev/gpio-hal.h"
#include "contiki.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dht11-sensor.h"

PROCESS(dht11_reader_process, "DHT11 Reader and LED Control Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

static struct etimer read_timer;

PROCESS_THREAD(dht11_reader_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&read_timer, 5 * CLOCK_SECOND);

  // Configure GPIO Port and Pin
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_A);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_HAL_PIN_0);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));

    // Start reading the sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Get the temperature and humidity values
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    // Print the temperature and humidity values
    printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

    // Turn on the RGB LED
    rgb_led_on();

    // Reset the timer
    etimer_reset(&read_timer);
  }

  PROCESS_END();
}