#include "contiki.h"
#include "dev/dht11.h"
#include <stdio.h>

static struct etimer timer;

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static int temperature, humidity;

  PROCESS_BEGIN();
  
  // initialize etimer to fire every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    // Read values from DHT11 sensor
    temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    humidity = dht11_sensor.value(DHT11_SENSOR_HUM);

    // Check if readings are valid
    if(temperature == DHT11_ERROR || humidity == DHT11_ERROR) {
      printf("Error reading from DHT11 sensor\n");
    } else {
      printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
    }

    // Reset the timer for the next round
    etimer_reset(&timer);
  }

  PROCESS_END();
}