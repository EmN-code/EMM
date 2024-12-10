#include "netstack.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include <stdio.h>
#include "sys/etimer.h"
#include "dev/rgb-led.h"
#include "dev/dht11.h"

PROCESS(nrf52840_dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&nrf52840_dht11_rgb_led_process);

PROCESS_THREAD(nrf52840_dht11_rgb_led_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Set a timer to fire every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    // Reset the timer
    etimer_reset(&timer);

    // Read data from DHT11 sensor
    if(dht11_read(&temperature, &humidity) != 0) {
      printf("Error reading DHT11\n");
    } else {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

      // Turn on RGB LED
      rgb_led_set(RGB_LED_WHITE);
    }
  }

  PROCESS_END();
}