#include "contiki.h"
#include "pt.h"
#include "process.h"
#include "lc-switch.h"
#include <stdio.h>
#include "dev/dht11-sensor.h"
#include "dev/rgb-led.h"

PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

static struct etimer timer;

PROCESS_THREAD(dht11_rgb_led_process, ev, data) {
  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    int temperature = DHT11_VALUE_TEMPERATURE_INTEGER;
    int humidity = DHT11_VALUE_HUMIDITY_INTEGER;

    if (dht11_sensor.value(DHT11_STATUS_OKAY) == DHT11_STATUS_OKAY) {
      printf("Temperature: %d C, Humidity: %d%\n", temperature, humidity);

      // Example of turning on RGB LED to white
      RGB_LED_WHITE;
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }

    // Reset the timer to trigger again after 5 seconds
    etimer_reset(&timer);
  }
  
  PROCESS_END();
}