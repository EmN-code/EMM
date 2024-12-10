#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/simple-udp.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"

extern const struct sensors_sensor dht11_sensor;
#include <stdio.h>
#include "sys/etimer.h"

#define RED_LED_PIN   17
#define GREEN_LED_PIN 18
#define BLUE_LED_PIN  19

static void rgb_led_on(void) {
  // Simulated function to turn on RGB LED, normally this would
  // include hardware-specific code to control GPIO pins
  printf("RGB LED ON\n");
}

PROCESS(read_dht11_and_control_led, "Read DHT11 and Control LED");
AUTOSTART_PROCESSES(&read_dht11_and_control_led);

PROCESS_THREAD(read_dht11_and_control_led, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5); // Setup the timer to trigger every 5 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    int temperature = 0;
    int humidity = 0;

    // Simulate reading from the DHT11 sensor
    if (dht11_sensor.value) {
      temperature = dht11_sensor.value(0); // Assume index 0 for temperature
      humidity = dht11_sensor.value(1); // Assume index 1 for humidity
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }

    rgb_led_on(); // Turn on the RGB LED

    etimer_reset(&timer); // Reset the timer for the next cycle
  }

  PROCESS_END();
}