#include <stdio.h>
#include "contiki.h"
#include "dev/dht11.h"
#include "dev/gpio-hal.h"
#include "dev/gpio.h"

PROCESS(dht11_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  SENSORS_ACTIVATE(dht11);

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Trigger DHT11 read
    if(dht11.value(DHT11_READ) == DHT11_ERROR) {
      printf("Error, could not read DHT11 sensor\n");
    } else {
      printf("Temperature: %d Celsius, Humidity: %d%%\n",
             dht11.value(DHT11_READ_TEMP),
             dht11.value(DHT11_READ_HUM));
    }
  }

  // Deactivate the sensor (optional, depending on your use case)
  SENSORS_DEACTIVATE(dht11);

  PROCESS_END();
}