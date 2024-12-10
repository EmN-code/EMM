#include "sensors_sensor.h"
#include "netstack.h"
#include "contiki-default-conf.h"

PROCESS(dht11_led_process, "DHT11 and LED process");
AUTOSTART_PROCESSES(&dht11_led_process);

static struct etimer et;

PROCESS_THREAD(dht11_led_process, ev, data)
{
  PROCESS_BEGIN();
  
  while (1) {
    // Set timer for 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);
    
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read the DHT11 sensor temperature and humidity
    int temperature, humidity;
    if (dht11_sensor.value(&temperature, &humidity) == 0) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

      // Turn on the RGB LED
      // Assume there is a function to control the RGB LED; pseudo-code given below:
      rgb_led_on();
      
      // Keep the LED on for demonstration purposes or adjust as needed
      clock_delay_usec(500000);
      
      // Turn off the RGB LED
      rgb_led_off();
    }
  }

  PROCESS_END();
}