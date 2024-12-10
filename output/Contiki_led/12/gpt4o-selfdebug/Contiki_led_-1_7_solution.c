#include "sys/etimer.h"
#include "sys/process.h"
#include <stdio.h>

#define DHT11_VALUE_TEMPERATURE_INTEGER (2)
#define DHT11_VALUE_TEMPERATURE_DECIMAL (3)
#define DHT11_VALUE_HUMIDITY_INTEGER (0)
#define DHT11_VALUE_HUMIDITY_DECIMAL (1)
#define RGB_LED_WHITE (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

PROCESS(example_process, "DHT11 Reader");
AUTOSTART_PROCESSES(&example_process);

PROCESS_THREAD(example_process, ev, data) {
  static struct etimer timer;

  PROCESS_BEGIN();
  
  // Set timer for 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);
  
  while (1) {
    // Wait for the timer event
    PROCESS_YIELD_UNTIL(etimer_expired(&timer));

    // Code to read DHT11 data
    printf("Reading DHT11 sensor...\n");
    // Placeholder for actual sensor reading logic
    int temperature = DHT11_VALUE_TEMPERATURE_INTEGER;
    int humidity = DHT11_VALUE_HUMIDITY_INTEGER;
    
    printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

    // Logic to turn on RGB LED
    printf("Turning on RGB LED\n");
    // Placeholder for code to control the RGB LED

    // Reset timer for next event
    etimer_reset(&timer);
  }

  PROCESS_END();  
}
