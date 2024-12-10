
#include "contiki.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include "dev/dht11-sensor.h"  // Ensure this header is available

#include <stdio.h>

#define SENSOR_INTERVAL (5 * CLOCK_SECOND)

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
  static struct etimer timer;
  uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(dht11_sensor);

  etimer_set(&timer, SENSOR_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    dht11_sensor.value_integer = DHT11_VALUE_HUMIDITY_INTEGER;
    humidity_integer = dht11_sensor.value_integer;

    dht11_sensor.value_integer = DHT11_VALUE_HUMIDITY_DECIMAL;
    humidity_decimal = dht11_sensor.value_integer;

    dht11_sensor.value_integer = DHT11_VALUE_TEMPERATURE_INTEGER;
    temperature_integer = dht11_sensor.value_integer;

    dht11_sensor.value_integer = DHT11_VALUE_TEMPERATURE_DECIMAL;
    temperature_decimal = dht11_sensor.value_integer;

    printf("Humidity: %d.%d, Temperature: %d.%d\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);

    leds_toggle(LEDS_RED);  // Toggle the RGB LED

    etimer_reset(&timer);
  }

  PROCESS_END();
}
