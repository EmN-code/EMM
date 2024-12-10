#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"

PROCESS(dht11_led_process, "DHT11 and LED control process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  static int temperature;
  static int humidity;

  PROCESS_BEGIN();

  while(1) {
    // Set the timer to expire after 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    SENSORS_ACTIVATE(dht11_sensor);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    SENSORS_DEACTIVATE(dht11_sensor);

    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

    // Turn on RGB LED to white if sensor read is successful
    if(temperature != -1 && humidity != -1) {
      leds_on(LEDS_RED);
      leds_on(LEDS_GREEN);
      leds_on(LEDS_BLUE);
    }

    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  }

  PROCESS_END();
}