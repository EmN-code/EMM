#include "dht11_sensor.h"
#include "contiki.h"

PROCESS(dht11_led_process, "DHT11 LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static struct etimer timer;

PROCESS_THREAD(dht11_led_process, ev, data) {
  PROCESS_BEGIN();
  
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read data from DHT11 sensor
    int humidity_integer = DHT11_VALUE_HUMIDITY_INTEGER;
    int temperature_integer = DHT11_VALUE_TEMPERATURE_INTEGER;

    printf("Humidity: %d\n", humidity_integer);
    printf("Temperature: %d\n", temperature_integer);

    // Turn on the RGB LED
    printf("RGB LED ON\n");

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}