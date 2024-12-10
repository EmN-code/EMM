#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data)
{
  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);   // Assuming GPIO Port 0
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);    // GPIO Pin 2

  while (1) {
    // Set the timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    // Activate the sensor to take a reading
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Check the status
    if (dht11_sensor.status(SENSORS_READY) == DHT11_STATUS_OKAY) {
      int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
      int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
      printf("Humidity: %d%%, Temperature: %dC\n", humidity, temperature);
    }
    else {
      printf("Error reading the sensor\n");
    }
    
    // Reset the timer for the next cycle
    etimer_reset(&timer);
  }

  PROCESS_END();
}
