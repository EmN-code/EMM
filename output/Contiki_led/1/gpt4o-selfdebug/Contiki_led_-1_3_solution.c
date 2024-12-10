#include "contiki-lib.h"
#include "contiki-default-conf.h"

#include <stdio.h>

#define RGB_LED_RED 0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE 0x04
#define RGB_LED_WHITE (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

extern const struct sensors_sensor dht11_sensor;

PROCESS(read_dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&read_dht11_process);

static struct etimer timer;

PROCESS_THREAD(read_dht11_process, ev, data)
{
  PROCESS_BEGIN();
  
  // Set the timer for 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    // Read from dht11_sensor
    int temp = dht11_sensor.value(0);
    int hum = dht11_sensor.value(1);
    printf("Temperature: %d Humidity: %d\n", temp, hum);
    
    // Turn on RGB LED as white
    printf("Turning on the RGB LED to white\n");
    // You would normally set the GPIO pins here to turn on the LED
    
    // Reset the timer
    etimer_reset(&timer);
  }
  
  PROCESS_END();
}

int main() {
  // Initialize and run the Contiki OS
  process_init();
  process_start(&etimer_process, NULL);
  autostart_start(autostart_processes);
  
  while(1) {
    do {
    } while(process_run() > 0);
  }
}