#include "node-id.h"
#include "net/ipv6/simple-udp.h"
#include "core/net/ipv6/uip-ds6.h"
#include "mqtt.h"
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/leds.h"

#define INTERVAL (5 * CLOCK_SECOND)

PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static struct etimer timer;

PROCESS_THREAD(dht11_led_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&timer, INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    SENSORS_ACTIVATE(dht11_sensor);

    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUM);

    printf("Temperature: %d C, Humidity: %d %\n", temperature, humidity);

    // Turn on the RGB LED
    leds_on(LEDS_RED);
    leds_on(LEDS_GREEN);
    leds_on(LEDS_BLUE);

    // Wait for a short period
    etimer_set(&timer, CLOCK_SECOND); // Wait 1 second with the LED on
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Turn off the RGB LED
    leds_off(LEDS_RED);
    leds_off(LEDS_GREEN);
    leds_off(LEDS_BLUE);

    etimer_reset(&timer);
  }

  PROCESS_END();
}