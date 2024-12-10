#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/process.h"
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "dht11-sensor.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer et;

PROCESS_THREAD(dht11_process, ev, data)
{
  PROCESS_BEGIN();

  // Configure the GPIO pin for the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_A);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_HAL_PIN_0);

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read the sensor
    SENSORS_ACTIVATE(dht11_sensor);
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    SENSORS_DEACTIVATE(dht11_sensor);

    // Output the readings (would typically use printf, omitted per instructions)
    // printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

    // Turn on the RGB LED
    leds_on(LEDS_RED);

    // Optionally, turn off the LED after a brief period for visual effect
    // etimer_set(&et, CLOCK_SECOND);
    // PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    // leds_off(LEDS_RED);
  }

  PROCESS_END();
}