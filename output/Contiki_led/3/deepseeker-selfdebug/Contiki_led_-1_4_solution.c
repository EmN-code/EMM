
#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(dht11_sensor);

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);

    leds_set(LEDS_RED); // Turn on the RGB LED

    etimer_reset(&timer);
  }

  PROCESS_END();
}
