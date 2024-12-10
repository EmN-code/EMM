#include "sensors.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include <stdio.h>

// Define the DHT11 sensor parameters
#define SENSOR_PIN 2 // Use GPIO 2 for DHT11

// Correctly define these missing configuration constants
#define DHT11_CONFIGURE_GPIO_PIN SENSOR_PIN
#define DHT11_VALUE_TEMPERATURE_INTEGER 2
#define DHT11_VALUE_HUMIDITY_INTEGER 0

static struct etimer timer;

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  PROCESS_BEGIN();
  
  // Initial timer setup to fire every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) 
  {
    // Wait for the timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Connect to the DHT11 sensor and read values
    int temperature = 0; // Placeholder: Replace with actual sensor reading
    int humidity = 0;    // Placeholder: Replace with actual sensor reading
    
    printf("Temperature: %d\n", temperature);
    printf("Humidity: %d\n", humidity);
    
    // Reset the timer to fire again in 5 seconds
    etimer_reset(&timer);
  }

  PROCESS_END();
}