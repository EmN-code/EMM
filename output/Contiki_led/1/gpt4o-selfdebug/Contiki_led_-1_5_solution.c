#include "sys/etimer.h"
#include "contiki.h"
#include "dev/dht11.h"
#include "dev/rgb_led.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Initiate DHT11 read
    if(dht11_read(&temperature, &humidity) == DHT11_STATUS_OK) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
      
      // Turn on RGB LED based on a condition, for now, always on
      rgb_led_set(RGB_LED_WHITE);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}